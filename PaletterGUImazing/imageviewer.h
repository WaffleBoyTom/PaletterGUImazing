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

    void openNautilus();
    bool loadImage(const QString *filename);
private:

    QVBoxLayout *myLayout;
    QLineEdit *myLineEdit;
    QLabel *myImageHolder;
    QPixmap *myImageDisplay;
    QPushButton *myNautilusButton; 
    QWidget *myCreator;
    
};

#endif
