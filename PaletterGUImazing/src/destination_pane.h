#ifndef DST_IMAGE_VIEWER_H
#define DST_IMAGE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick/sick_dropdown.h"
#include "sick/sick_file_line_edit.h"
#include "sick/sick_image_viewer.h"

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

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:

    // Loads an image with the specified path.
    void loadImage(const QString &file_path);

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

    // Original unfiltered image. Successive remaps are applied to this image.
    QImage myUnfilteredImage;

    // Displayed image.
    QImage myImage;

    // Calls resetImage.
    QPushButton *myResetButton;

    // Calls applyPalette.
    QPushButton *myApplyPaletteButton;

    // Remap method.
    SickDropDown *myRemapMethodDropdown;

    // Execution provider.
    SickDropDown *myExecutionProviderDropdown;
};

#endif  // DST_IMAGE_VIEWER_H
