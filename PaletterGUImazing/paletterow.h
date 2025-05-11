#ifndef PALETTEROW_H
#define PALETTEROW_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QWidget;
class QVBoxLayout;
class QPushButton;
QT_END_NAMESPACE

class PaletteRow : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteRow(QWidget *parent);

    QSize sizeHint() const override;

public slots:
    void onPaletteCountChanged(const int count);
    void drawPalette(QList<QColor> *palette);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVBoxLayout *myLayout;
    QWidget *myCreator;

    QList<QColor> *myPalettePtr;
    int myBoxCount;
};

#endif
