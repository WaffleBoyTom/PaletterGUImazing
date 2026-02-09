#ifndef PALETTER_WINDOW_H
#define PALETTER_WINDOW_H

#include <QWidget>

#include "image_viewer.h"
#include "palette_viewer.h"
#include "sick_log_viewer.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;
class QLineEdit;
QT_END_NAMESPACE

class PaletterGUI : public QWidget
{
    Q_OBJECT

public:
    PaletterGUI();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void setPaletteCount(int count);
    void drawPalette(QList<QColor> *palette);
    void applyPaletteToSecondViewer();
    void logMeHard(const QString msg);

private:
    QLabel *paletterLabel;
    ImageViewer *myImgViewer;
    PaletteViewer *myPaletteViewer;
    ImageViewer *myConvertImgViewer;
    SickLogViewer *myLogViewer;
};

#endif  // PALETTER_WINDOW_H
