#ifndef PALETTERWINDOW_H
#define PALETTERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class PaletterWindow;
}
QT_END_NAMESPACE

class PaletterWindow : public QMainWindow
{
    Q_OBJECT

public:
    PaletterWindow(QWidget *parent = nullptr);
    ~PaletterWindow();

private:
    Ui::PaletterWindow *ui;
};
#endif // PALETTERWINDOW_H
