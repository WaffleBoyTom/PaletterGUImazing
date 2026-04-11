#ifndef PALETTE_ROW_H
#define PALETTE_ROW_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>
#include "sick_export_opts.h"

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
    enum class PaletteDrawStyle
    {
        RECT,
        APPLE,   // they make everything super round don't they
        VK,      // if you wish to rasterize a triangle, 
                 // you must first reinvent the universe
                 // disabled as it looks baaad
        INVALID
    };
    explicit PaletteRow(QWidget *parent);

    QSize sizeHint() const override;
    // fills json with palette
    // returns false if the palette is empty
    bool serialize(QJsonObject &json, SickExportOpts::ExportFormat fmt);

public slots:
    void onPaletteDisplaySizeChanged(int size);
    void onPaletteChanged(QList<QColor> *palette);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    std::optional<QColor> findColor(QPoint position) const;

    QVBoxLayout *myLayout;

    QList<QColor> *myPalette;
    int myBoxCount;
    PaletteDrawStyle myDrawStyle;
};

#endif  // PALETTE_ROW_H
