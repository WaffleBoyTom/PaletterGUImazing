#include "paletterwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    PaletterGUI window;
    
    window.show();
    
    return app.exec();
}
