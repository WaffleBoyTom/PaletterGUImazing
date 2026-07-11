#ifndef SICK_INSPECTOR_H
#define SICK_INSPECTOR_H

#include <QtWidgets>

class SickInspector : protected QWidget
{
    Q_OBJECT
public:
    static void displayColor(QColor color, QPoint position);
    static void hideColor();

    void paintEvent(QPaintEvent *event) override;

private:
    static SickInspector *getInstance();

    SickInspector();

    static SickInspector *theInspector;

    QColor myColor;

    /// probably should be enum..
    bool myIsRgbFDisplay = true;

};

#endif  // SICK_INSPECTOR_H
