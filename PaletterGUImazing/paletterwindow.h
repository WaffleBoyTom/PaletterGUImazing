#ifndef PALETTERWINDOW_H
#define PALETTERWINDOW_H

#include <QWidget>

#include "imageviewer.h"
#include "paletteviewer.h"

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
    // QPushButton *newTestButton;
    // QLineEdit *newLineEdit;
    ImageViewer *myImgViewer;
    PaletteViewer *myPaletteViewer;
    ImageViewer *myConvertImgViewer;
    QLabel *myLogger;
};

#endif  // PALETTERWINDOW_H
