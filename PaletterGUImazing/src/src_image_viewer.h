#ifndef SRC_IMAGE_VIEWER_H
#define SRC_IMAGE_VIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "sick_dropdown.h"
#include "sick_imageholder.h"
#include "sick_file_line_edit.h"

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

class SrcImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SrcImageViewer(QWidget *parent);
    void handleResizing();
    void setPaletteDisplaySize(int size);
    QList<QColor> *palette();


    constexpr static int INIT_PALETTE_SIZE = 6;

signals:
    void tellBossAboutPaletteFill(QList<QColor> *palette);

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

    // Resizes image based on parent size.
    QPixmap resizeImage(QPixmap *image, int width, int height);

private:
    QVBoxLayout *myLayout;

    SickFileLineEdit *myLineEdit;

    SickImageHolder  *myImageHolder;

    // keep reference to original image otherwise
    // we iteratively scale the pixmap
    // and end up with mashed pixeloes
    // and they aint delicious...
    QPixmap myLoadedImage;

    // calls processImage
    QPushButton *myProcessorButton;

    // dropdown for processing method
    SickDropDown *myModeDropdown;

    // dropdown for specifying device
    SickDropDown *myDeviceDropdown;

    QList<QColor> myPalette;
    int myPaletteDisplaySize;
};

#endif  // SRC_IMAGE_VIEWER_H
