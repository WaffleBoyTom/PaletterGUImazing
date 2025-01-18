#include "paletterwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaletterWindow w;
    w.show();
    return a.exec();
}
