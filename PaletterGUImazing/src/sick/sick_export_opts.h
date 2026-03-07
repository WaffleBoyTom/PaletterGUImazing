#ifndef SICK_EXPORT_OPTS_H
#define SICK_EXPORT_OPTS_H

#include <QtWidgets>


class SickExportOpts : public QWidget
{
    Q_OBJECT

public:
    SickExportOpts(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;
    // void mouseMoveEvent(QMouseEvent *event) override;
    // void leaveEvent(QEvent *event) override;
    // void mouseReleaseEvent(QMouseEvent *event) override;


};

#endif  // SICK_EXPORT_OPTS_H
