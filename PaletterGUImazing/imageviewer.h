#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QPixmap;
QT_END_NAMESPACE

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent);

private slots:

    void openNautilus();

private:

    QHBoxLayout *myLayout;
    QLineEdit *myLineEdit;
    QLabel *myImageHolder;
    QPixmap *myImageDisplay;
    QPushButton *myNautilusButton; 
    
};

#endif
