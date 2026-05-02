#include <QApplication>

#include "paletter_window.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    // set base color for all widgets
    // but this gets overriden per widget
    app.setStyleSheet("* { color: cornsilk; background-color: #232323; }");
    app.setStyleSheet(
        "QPushButton { color: cornsilk; background-color: #191919 }"
    );
    PaletterGUI window;
    window.setWindowFlags(Qt::Window);
    window.show();

    return app.exec();
}
