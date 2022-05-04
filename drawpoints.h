#ifndef DRAWPOINTSINGLETON_H
#define DRAWPOINTSINGLETON_H

#include <QVector>
#include <QPoint>

#include "drawSetting.h"

class drawPointSingleton
{
    friend class LScreenCaputrerUI;
public:
    enum drawAction{
        CREATE,
        MOVE,

        TOP_EXPAND,
        BOTTOM_EXPAND,
        LEFT_EXPAND,
        RIGHT_EXPAND,

        TOP_LEFT_EXPAND,
        BOTTOM_LEFT_EXPAND,
        TOP_RIGHT_EXPAND,
        BOTTOM_RIGHT_EXPAND,

        PAINTING
    };

    struct drawParam{
        drawAction action;
        drawSetting setting;
        QVector<QPoint> path;
    };

private:
    drawPointSingleton();
    drawPointSingleton(const drawPointSingleton& other);
    static drawPointSingleton* m_instance;

public:
    static drawPointSingleton* getInstance();

public:
    drawParam& current();
    QList<drawParam>& history();

private:
    void drawInit();
    void appendPoint(const QPoint& point);
    void appendfPaintPoint(const QPoint& point);
    void setCurrentAction(drawPointSingleton::drawAction action);
    void setCurrentSetting(const drawSetting& setting);
    void appendHistory();
    void resetAll();
    void resetPath();

private:
    drawParam m_currentDraw;
    QList<drawParam> m_drawList;
};

#endif // DRAWPOINTSINGLETON_H
