#include <QApplication>

#include "paletterwindow.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PaletterGUI window;

    window.show();

    return app.exec();
}
