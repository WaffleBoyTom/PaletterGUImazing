#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick_dropdown.h"
#include "sick_file_line_edit.h"
#include "sick_imageholder.h"

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent, bool paletteSource);
    void handleResizing();
    void setPaletteDisplaySize(int size);
    QList<QColor> *palette();

    // Apply the palette to the image.
    void applyPalette(QList<QColor> *palette);

    constexpr static int INIT_PALETTE_SIZE = 6;

signals:
    void tellBossAboutPaletteFill(QList<QColor> *palette);
    void askBossForPalette();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    // loads image from file explorer into window
    void loadImage(const QString &filename);
    // called when the editingFinished is fired by line edit
    void loadImageFromLineEdit();

    // get QImage from image holder
    QImage getImage();

    // resizes image based on drag
    void resizeOnDrag(int width, int height);

    // Generate a new palette from the image.
    void generatePalette();

    // Called after palette is generated.
    void onGeneratePaletteFinished(QList<QColor> palette);

    // Called after palette apply completes.
    void onApplyPaletteFinished(QImage image);

    void askForPalette();

    // Resizes image based on parent size.
    QPixmap resizeImage(QPixmap *image, int width, int height);

private:
    QVBoxLayout *myLayout;

    SickFileLineEdit *myLineEdit;

    SickImageHolder *myImageHolder;

    // keep reference to original image otherwise
    // we iteratively scale the pixmap
    // and end up with mashed pixeloes
    // and they aint delicious...
    QPixmap myLoadedImage;

    // // calls openNautilus
    // QPushButton *myNautilusButton;

    // calls processImage
    QPushButton *myProcessorButton;

    // dropdown for processing method
    SickDropDown *myModeDropdown;

    // dropdown for specifying device
    SickDropDown *myDeviceDropdown;

    QList<QColor> myPalette;
    int myPaletteDisplaySize;
};

#endif  // IMAGE_VIEWER_H
