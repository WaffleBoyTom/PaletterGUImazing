#include "quantizer.h"

#include <QDebug>

namespace
{

// Indicates a channel in an RGB color value.
enum class Channel
{
    Red = 0,
    Green,
    Blue
};

// A channel together with its maximum delta. This is just a helper struct used
// by the maxChannelDelta function.
struct ChannelDelta
{
    Channel chan;
    int delta;

    ChannelDelta(const Channel chan, const int delta) : chan(chan), delta(delta)
    {
    }
};

// A bucket, represented as a single offset into the input array. We *could*
// split all the colors into real containers, but using an offset is safe and
// cheap. We also cache the maximum channel delta value here so we don't need to
// recompute it for all buckets on every iteration.
struct Bucket
{
    std::size_t start;
    Channel chan;
    int delta;

    Bucket(const std::size_t start, const Channel chan, const int delta)
        : start(start), chan(chan), delta(delta)
    {
    }
};

// Componentwise maximum over the RGB channels of a color. Used by
// maxChannelDelta as a convenience.
QColor
max(const QColor &a, const QColor &b)
{
    return QColor(
        std::max(a.red(), b.red()),
        std::max(a.green(), b.green()),
        std::max(a.blue(), b.blue())
    );
}

// Componentwise minimum over the RGB channels of a color. Used by
// maxChannelDelta as a convenience.
QColor
min(const QColor &a, const QColor &b)
{
    return QColor(
        std::min(a.red(), b.red()),
        std::min(a.green(), b.green()),
        std::min(a.blue(), b.blue())
    );
}

// Determines the channel with the maximum difference, returning that channel
// and the difference. Corresponds to step 2. from the header comment.
ChannelDelta
maxChannelDelta(QSpan<const QColor> colors)
{
    QColor high = colors.front();
    QColor low = colors.front();

    for (const QColor &color : colors)
    {
        high = max(high, color);
        low = min(low, color);
    }

    const QColor delta(
        high.red() - low.red(),
        high.green() - low.green(),
        high.blue() - low.blue()
    );

    if (delta.red() > delta.green() && delta.red() > delta.blue())
        return ChannelDelta(Channel::Red, delta.red());
    else if (delta.green() > delta.red() && delta.green() > delta.blue())
        return ChannelDelta(Channel::Green, delta.green());
    else
        return ChannelDelta(Channel::Blue, delta.blue());
}

// Determines the average value of all the colors in a bucket (represented by a
// QSpan). Corresponds the second half of step 6. in the header comment.
QColor
average(QSpan<const QColor> colors)
{
    quint64 r = 0, g = 0, b = 0;

    for (const QColor &color : colors)
    {
        r += static_cast<quint64>(color.red());
        g += static_cast<quint64>(color.green());
        b += static_cast<quint64>(color.blue());
    }

    const float size = static_cast<float>(colors.size());
    return QColor(
        static_cast<int>(std::round(static_cast<float>(r) / size)),
        static_cast<int>(std::round(static_cast<float>(g) / size)),
        static_cast<int>(std::round(static_cast<float>(b) / size))
    );
}

// Sorts a bucket of colors along the specified channel. We use radix
// sort/counting sort/bucket sort, since I found it improved runtime by several
// factors over standard comparative methods. Used for step 4. of the header
// comment.
//
// There's a pretty good explanation on wikipedia
// (https://en.wikipedia.org/wiki/Radix_sort), but I'll try to explain it below.
//
// Since we're sorting RGB colors, each channel only can take on 256 possible
// values (0 through 255). Thus we can make an array, called a bucket (***this
// is completely different to the bucket used by median cut***) for each of
// these possible values. Then, for each color, we just plop it into the bucket
// corresponding to the desired channel.
//
// So if we had the color [123, 0, 284], and are sorting by the red channel, it
// would go into the 123rd bucket.
//
// Then, we just flatten all 255 buckets into a single array, and the result
// will be sorted by that channel
void
sortByChannel(QSpan<QColor> colors, const Channel chan)
{
    QVector<QVector<QColor>> buckets(256);

    // Add all the colors to their buckets.
    switch (chan)
    {
    case Channel::Red:
    {
        for (const QColor &color : colors)
            buckets[color.red()].push_back(color);
        break;
    }
    case Channel::Green:
    {
        for (const QColor &color : colors)
            buckets[color.green()].push_back(color);
        break;
    }
    case Channel::Blue:
    {
        for (const QColor &color : colors)
            buckets[color.blue()].push_back(color);
        break;
    }
    default:
        Q_UNREACHABLE();
    }

    // Flatten the buckets into a single array and return it.
    int idx = 0;
    for (const auto &bucket : buckets)
    {
        for (const QColor &color : bucket)
            colors[idx++] = color;
    }
}

}

Quantizer::Quantizer(int palette_size, Method method)
    : myPaletteSize(palette_size), myMethod(method)
{
}

/// The main loop of median cut. The while loop will run myPaletteSize times.
/// See the header comment for details.
QVector<QColor>
Quantizer::generatePalette(const QImage &image) const
{
    // Flatten the image into a 1D array of colors.
    QVector<QColor> colors_vec;
    colors_vec.reserve(image.width() * image.height());

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            colors_vec.push_back(image.pixelColor(x, y));
        }
    }

    // Take a span of the colors. Spans are awesome, they're essentially slices
    // from other languages. Internally, they are just an offset (into some
    // underlying container, which in this case is a QVector), plus a length. So
    // they're super cheap to pass around and let us change our "view" into a
    // container easily.
    QSpan<QColor> colors = colors_vec;

    // The buckets (!)
    QVector<Bucket> buckets;
    buckets.reserve(myPaletteSize + 1);

    // Determine the first bucket.
    const ChannelDelta cd = maxChannelDelta(colors);
    buckets.push_back(Bucket(0, cd.chan, cd.delta));

    // Add a sentinel bucket to assist in splitting at the back of the buffer.
    // This doesn't actually correspond to a color, but it does make the
    // implementation simpler.
    buckets.push_back(Bucket(colors.size(), cd.chan, 0));

    // The main palette construction loop. After each iteration, we have a new
    // bucket.
    while (buckets.size() <= myPaletteSize)
    {
        // Find the bucket with the maximum channel difference (step 3.). We use
        // the standard library's max_element algorithm to get an iterator to
        // the element. We have to use a custom comparator (the lambda) to sort
        // the buckets by their deltas.
        auto result = std::max_element(
            buckets.begin(),
            buckets.end(),
            [](const Bucket &a, const Bucket &b) { return a.delta < b.delta; }
        );

        // Get the indices for splitting the bucket. We use iterator tomfoolery
        // to get the start index of the next bucket, and use it to compute the
        // end index of the max element bucket.
        const std::size_t start = result->start;
        const std::size_t end = (result + 1)->start;
        const std::size_t mid = (start + end) / 2;

        // Sort the maximum delta bucket by the maximum channel.
        QSpan<QColor> bucket_colors = colors.subspan(start, end - start);
        sortByChannel(bucket_colors, result->chan);

        // Determine the maximum channel differences of the two newly created
        // buckets.
        const ChannelDelta cd0 =
            maxChannelDelta(colors.subspan(start, mid - start));
        const ChannelDelta cd1 =
            maxChannelDelta(colors.subspan(mid, end - mid));

        // Create a new bucket that starts where the previous one did, as well
        // as a new bucket at the median.
        *result = Bucket(start, cd0.chan, cd0.delta);
        buckets.insert(result + 1, Bucket(mid, cd1.chan, cd1.delta));
    }

    // Take the average RGB value of all the buckets and slap them into a
    // vector. Return it as our palette.
    QVector<QColor> palette;
    for (int i = 0; i < buckets.size() - 1; ++i)
    {
        const std::size_t start = buckets[i].start;
        const std::size_t len = buckets[i + 1].start - start;
        QSpan<const QColor> bucket_colors = colors.subspan(start, len);
        palette.push_back(average(bucket_colors));
    }
    return palette;
}
