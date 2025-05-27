#include "quantizer.h"

#include <QDebug>

namespace
{

enum class Channel
{
    Red = 0,
    Green,
    Blue
};

struct ChannelDelta
{
    Channel chan;
    int delta;

    ChannelDelta(const Channel chan, const int delta) : chan(chan), delta(delta)
    {
    }
};

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

QColor
max(const QColor &a, const QColor &b)
{
    return QColor(
        std::max(a.red(), b.red()),
        std::max(a.green(), b.green()),
        std::max(a.blue(), b.blue())
    );
}

QColor
min(const QColor &a, const QColor &b)
{
    return QColor(
        std::min(a.red(), b.red()),
        std::min(a.green(), b.green()),
        std::min(a.blue(), b.blue())
    );
}

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

void
sortByChannel(QSpan<QColor> colors, const Channel chan)
{
    QVector<QVector<QColor>> buckets(256);

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

    int idx = 0;
    for (const auto &bucket : buckets)
    {
        for (const QColor &color : bucket)
            colors[idx++] = color;
    }
}

}

Quantizer::Quantizer(const int palette_size) : myPaletteSize(palette_size)
{
}

QVector<QColor>
Quantizer::generatePalette(const QImage &image) const
{
    QVector<QColor> colors_vec;
    colors_vec.reserve(image.width() * image.height());

    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            colors_vec.push_back(image.pixelColor(x, y));
        }
    }

    QSpan<QColor> colors = colors_vec;

    QVector<Bucket> buckets;
    buckets.reserve(myPaletteSize + 1);

    const ChannelDelta cd = maxChannelDelta(colors);
    buckets.push_back(Bucket(0, cd.chan, cd.delta));

    // Sentinel bucket to assist in splitting at the back of the buffer.
    buckets.push_back(Bucket(colors.size(), cd.chan, 0));

    while (buckets.size() <= myPaletteSize)
    {
        auto result = std::max_element(
            buckets.begin(),
            buckets.end(),
            [](const Bucket &a, const Bucket &b) { return a.delta < b.delta; }
        );

        const std::size_t start = result->start;
        const std::size_t end = (result + 1)->start;
        const std::size_t mid = (start + end) / 2;

        QSpan<QColor> bucket_colors = colors.subspan(start, end - start);
        sortByChannel(bucket_colors, result->chan);

        const ChannelDelta cd0 =
            maxChannelDelta(colors.subspan(start, mid - start));
        const ChannelDelta cd1 =
            maxChannelDelta(colors.subspan(mid, end - mid));

        *result = Bucket(start, cd0.chan, cd0.delta);
        buckets.insert(result + 1, Bucket(mid, cd1.chan, cd1.delta));
    }

    QVector<QColor> palette;
    for (int i = 0; i < buckets.size() - 1; ++i)
    {
        const Bucket &bucket = buckets[i];
        const std::size_t start = buckets[i].start;
        const std::size_t len = buckets[i + 1].start - start;
        QSpan<const QColor> bucket_colors = colors.subspan(start, len);
        palette.push_back(average(bucket_colors));
    }
    return palette;
}
