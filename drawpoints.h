#ifndef DRAWPOINTSINGLETON_H
#define DRAWPOINTSINGLETON_H

#include "lscreencaputrer_ui.h"

#include <QVector>
#include <QPoint>

class drawPointSingleton
{
    friend class LScreenCaputrerUI;
public:
    enum drawAction{
        CREATE,
        MOVE,
        LEFT_TOP_EXPAND,
        LEFT_BOTTOM_EXPAND,
        RIGHT_TOP_EXPAND,
        RIGHT_BOTTOM_EXPAND
    };
    enum drawShape{
        NONE,
        FREE,
        LINE,
        RECTANGLE,
        ELLIPSE
    };
    struct drawParam{
        drawAction action;
        drawShape shape;
        QVector<QPoint> path;
        Qt::GlobalColor color;
    };

private:
    drawPointSingleton();
    drawPointSingleton(const drawPointSingleton& other);

public:
    static drawPointSingleton* getInstance();
    static drawPointSingleton* m_instance;

public:
    drawParam& current();
    QList<drawParam>& history();

private:
    void drawInit();
    void appendPoint(const QPoint& point);
    void setCurrentAction(drawPointSingleton::drawAction action);
    void appendHistory();
    void reset();

private:
    drawParam m_currentDraw;
    QList<drawParam> m_drawList;


};

#endif // DRAWPOINTSINGLETON_H
