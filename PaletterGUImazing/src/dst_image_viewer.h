#ifndef DST_IMAGE_VIEWER_H
#define DST_IMAGE_VIEWER_H

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

class DstImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DstImageViewer(QWidget *parent);
    void handleResizing();
    
    // Apply the palette to the image.
    void applyPalette(QList<QColor> *palette);

signals:
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

    // Called after palette apply completes.
    void onApplyPaletteFinished(QImage image);

    void askForPalette();
    
    // resizes image based on drag
    void resizeOnDrag(int width, int height);
    
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
    
    // image that we've applied the palette to 
    QPixmap myModifiedImage;

    // calls processImage
    QPushButton *myProcessorButton;

    // dropdown for processing method
    SickDropDown *myModeDropdown;

    // dropdown for specifying device
    SickDropDown *myDeviceDropdown;

};

#endif  // DST_IMAGE_VIEWER_H
