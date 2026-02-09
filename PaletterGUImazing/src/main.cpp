#include <QApplication>

#include "paletter_window.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    PaletterGUI window;

    window.show();

    return app.exec();
}
