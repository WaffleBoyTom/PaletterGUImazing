#ifndef PALETTERWINDOW_H
#define PALETTERWINDOW_H

#include <QPixmap>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;
QT_END_NAMESPACE

class PaletterGUI : public QWidget
{
    Q_OBJECT

public:
    PaletterGUI();

protected:
    //void resizeEvent(QResizeEvent *event) override;

private slots:
    void buttonTestAction();

private:

    QLabel *paletterLabel;
    QPushButton *newTestButton;
};

#endif // PALETTERWINDOW_H
