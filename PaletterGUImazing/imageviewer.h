#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QWidget>
#include "imageprocessor.h"

QT_BEGIN_NAMESPACE
class QAction;
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
    explicit ImageViewer(QWidget *parent);

private slots:
    
    // opens file explorer
    void openNautilus();
    
    // loads image from file explorer into window
    bool loadImage(const QString *filename);

    // get QImage from image holder
    QImage getImage();

    void processImage();
    

private:

    QVBoxLayout *myLayout;
    QLineEdit *myLineEdit;
    
    QLabel *myImageHolder;
    QPixmap *myImageDisplay;
    
    // calls openNautilus
    QPushButton *myNautilusButton; 

    // calls processImage
    QPushButton *myProcessorButton;
    
    // pointer to paletter window
    QWidget *myCreator;
    ImageProcessor myImageProcessor;
    
};

#endif
