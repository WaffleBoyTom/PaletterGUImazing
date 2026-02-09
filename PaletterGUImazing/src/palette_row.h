#ifndef PALETTE_ROW_H
#define PALETTE_ROW_H

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
    void onPaletteDisplaySizeChanged(int size);
    void onPaletteChanged(QList<QColor> *palette);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVBoxLayout *myLayout;

    QList<QColor> *myPalette;
    int myBoxCount;
};

#endif  // PALETTE_ROW_H
