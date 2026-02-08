#include <QApplication>

#include "paletter_window.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PaletterGUI window;

    window.show();

    return app.exec();
}
