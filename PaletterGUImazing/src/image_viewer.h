#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick_dropdown.h"

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QVBoxLayout;
class QLineEdit;
class QPushButton;
class QPixmap;
class QWidget;
QT_END_NAMESPACE

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent, bool paletteSource);
    void handleResizing();
    void setPaletteCount(const int count);
    QList<QColor> *getPalette();

    // Apply the palette to the image.
    void applyPalette(QList<QColor> *palette);

signals:

    void tellBossAboutPaletteFill(QList<QColor> *palette);
    void tellBossToLog(QString logmsg);
    void askBossForPalette();

private slots:

    // opens file explorer
    void openNautilus();

    // loads image from file explorer into window
    bool loadImage(const QString *filename);

    // get QImage from image holder
    QImage getImage();

    // Generate a new palette from the image.
    void generatePalette();

    // Called after palette is generated.
    void onGeneratePaletteFinished(QList<QColor> palette);

    // Called after palette apply completes.
    void onApplyPaletteFinished(QImage image);

    void askForPalette();

    // Resizes image based on parent size.
    QPixmap resizeImage(QPixmap *image);

private:
    QVBoxLayout *myLayout;

    QLineEdit *myLineEdit;

    QLabel *myImageHolder;

    // keep reference to original image otherwise
    // we iteratively scale the pixmap
    // and end up with mashed pixeloes
    // and they aint delicious...
    QPixmap myLoadedImage;

    // calls openNautilus
    QPushButton *myNautilusButton;

    // calls processImage
    QPushButton *myProcessorButton;

    // dropdown for processing method
    SickDropDown *myModeDropdown;

    // dropdown for specifying device
    SickDropDown *myDeviceDropdown;

    QList<QColor> myColorPalette;
    int myPaletteCount;
};

#endif  // IMAGE_VIEWER_H
