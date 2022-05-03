#ifndef DRAWSETTING_H
#define DRAWSETTING_H

#include <QObject>
#include <QColor>

struct drawSetting{
    enum drawShape{
        NONE,
        FREE,
        LINE,
        RECTANGLE,
        ELLIPSE
    };

    drawShape shape      = NONE;
    qint32    lineWidth  = -1;
    QColor    color      = Qt::black;
};

#endif // DRAWSETTING_H
