#ifndef SICKVIEWER_H
#define SICKVIEWER_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

#include "image_processor.h"
#include "sickdropdown.h"

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

class SickViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SickViewer(QWidget *parent);
    void handleResizing();

private slots:

    // opens file explorer
    void openNautilus();

    // loads image from file explorer into window
    bool loadImage(const QString *filename);

    // get QImage from image holder
    QImage getImage();

    // load into image processor and get output
    virtual void processImage() = 0;

    // resizes image based on myCreator size;
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

    // pointer to paletter window
    QWidget *myCreator;

    // deals with processing the pixmap
    ImageProcessor myImageProcessor;
};

#endif
