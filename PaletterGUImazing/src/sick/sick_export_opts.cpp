#include "sick_export_opts.h"
#include <QTimer>


SickExportOpts::SickExportOpts(QWidget *parent) 
: QWidget(parent)
{
    // QTimer *timer = new QTimer(this);
    // connect(
    //     timer, 
    //     &QTimer::timeout, 
    //     this, 
    //     QOverload<>::of(&SickPerfViewer::update)
    // );
    
    // timer->start(500); 
    setMinimumSize(screen()->geometry().width() / 2, 150);
}

void
SickExportOpts::paintEvent(QPaintEvent *event)
{
    int width = this->width() / 3;
    int padding = width / 10;
    int steps = width - (padding) / 3;
    int height = this->height();

    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(QColorConstants::Svg::cornsilk); 
    QFont pfont;
    QPen filledpen;
    filledpen.setStyle(Qt::NoPen);
    
    pen.setWidth(4);    
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);  
    
    const int rad = qMin(
        (height - 10) / 2,
        (width  - 20)  / 2  
    );
    pfont.setPixelSize(rad / 3);
    painter.setFont(pfont);

    QList<QString> devices({QString("Export"), QString("Options"), QString("Here")});
    for (int i = 0; i < 3; ++i)
    {            
        QPoint pos = QPoint(
            (width / 2) + (steps * i), 
            height / 2
        );
        painter.drawText(pos, devices[i]);

    
    }
}
