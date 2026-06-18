#ifndef DST_IMAGE_VIEWER_H
#define DST_IMAGE_VIEWER_H

#include <QtWidgets/qpushbutton.h>
#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick_dropdown.h"
#include "sick_file_line_edit.h"
#include "sick_imageholder.h"

class DstImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DstImageViewer(QWidget *parent);

    // Apply the palette to the image.
    void applyPalette(QList<QColor> *palette);

signals:
    void askBossForPalette();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    // loads image from file explorer into window
    void onLoadImage(const QString &filename);

    // called when the editingFinished is fired by line edit
    void onLoadImageFromLineEdit();

    // Called after palette apply completes.
    void onApplyPaletteFinished(QImage image);

    void askForPalette();

    // resizes image based on drag
    void resizeOnDrag(int width, int height);

    // Resets the image
    void resetImage();

private:

    void resetImageSize();

    void resizeImage(const int width, const int height);

    int initialImageWidth() const;
    int initialImageHeight() const;

    SickFileLineEdit *myLineEdit;

    SickImageHolder *myImageHolder;

    // keep reference to original image otherwise
    // we iteratively scale the pixmap
    // and end up with mashed pixeloes
    // and they aint delicious...
    QImage myUnfilteredImage;

    // image that we've applied the palette to
    QImage myImage;

    QPushButton *myResetButton;

    // calls processImage
    QPushButton *myProcessorButton;

    // dropdown for processing method
    SickDropDown *myModeDropdown;

    // dropdown for specifying device
    SickDropDown *myDeviceDropdown;
};

#endif  // DST_IMAGE_VIEWER_H
