#ifndef DST_IMAGE_VIEWER_H
#define DST_IMAGE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick_dropdown.h"
#include "sick_file_line_edit.h"
#include "sick_image_viewer.h"

class DestinationPane : public QWidget
{
    Q_OBJECT

public:
    explicit DestinationPane(QWidget *parent);

    // Apply the palette to the image.
    void applyPalette(QList<QColor> *palette);

signals:
    void askBossForPalette();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    // Called when an image selected from the file explorer is loaded.
    void onLoadImage(const QString &filename);

    // Called when editingFinished is fired by the line edit.
    void onLoadImageFromLineEdit();

    // Called after palette apply completes.
    void onApplyPaletteFinished(QImage image);

    void askForPalette();

    // Resets the image
    void resetImage();

private:

    SickFileLineEdit *myLineEdit;

    SickImageViewer *myImageViewer;

    // Original unfiltered image. Successive quantizations are applied
    // to this image.
    QImage myUnfilteredImage;

    // Displayed image.
    QImage myImage;

    // Calls resetImage.
    QPushButton *myResetButton;

    // Calls applyPalette.
    QPushButton *myApplyPaletteButton;

    // Quantization method.
    SickDropDown *myQuantizationMethodDropdown;

    // Processing device.
    SickDropDown *myDeviceDropdown;
};

#endif  // DST_IMAGE_VIEWER_H
