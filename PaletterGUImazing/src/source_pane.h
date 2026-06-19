#ifndef SRC_IMAGE_VIEWER_H
#define SRC_IMAGE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick_dropdown.h"
#include "sick_file_line_edit.h"
#include "sick_image_viewer.h"

class SourcePane : public QWidget
{
    Q_OBJECT

public:
    explicit SourcePane(QWidget *parent);

    // Retrieve the stored palette.
    QList<QColor> *palette();

    // Set the number of palette visualizers.
    void setPaletteDisplaySize(int size);

signals:
    void tellBossAboutPaletteFill(QList<QColor> *palette);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    // Called when an image selected from the file explorer is loaded.
    void onLoadImage(const QString &filename);

    // Called when editingFinished is fired by the line edit.
    void onLoadImageFromLineEdit();

    // Generate a new palette from the image.
    void generatePalette();

    // Called after palette is generated.
    void onGeneratePaletteFinished(QList<QColor> palette);

private:

    constexpr static int INIT_PALETTE_SIZE = 6;

    SickFileLineEdit *myLineEdit;

    SickImageViewer *myImageViewer;

    // Calls applyPalette.
    QPushButton *myProcessorButton;

    // Quantization method.
    SickDropDown *myQuantizationMethodDropdown;

    // Processing device.
    SickDropDown *myDeviceDropdown;

    // smn about mashed pixeloes, and they aint delicious...
    QImage myImage;

    QList<QColor> myPalette;
    int myPaletteDisplaySize;
};

#endif  // SRC_IMAGE_VIEWER_H
