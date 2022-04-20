#include "drawpoints.h"

drawPointSingleton* drawPointSingleton::m_instance = nullptr;

drawPointSingleton::drawPointSingleton()
{
    drawInit();
}

drawPointSingleton *drawPointSingleton::getInstance()
{
    if(m_instance == nullptr){
        m_instance = new drawPointSingleton();
    }
    return m_instance;
}

drawPointSingleton::drawParam &drawPointSingleton::current()
{
    return m_currentDraw;
}

QList<drawPointSingleton::drawParam> &drawPointSingleton::history()
{
    return m_drawList;
}
void drawPointSingleton::drawInit()
{
    m_currentDraw.color = Qt::black;
    m_currentDraw.path.clear();
    m_currentDraw.shape = NONE;
}

void drawPointSingleton::appendPoint(const QPoint &point)
{
    m_currentDraw.path.append(point);
}

void drawPointSingleton::setCurrentAction(drawPointSingleton::drawAction action)
{
    m_currentDraw.action = action;
}

void drawPointSingleton::appendHistory()
{
    m_drawList.append(m_currentDraw);
}

void drawPointSingleton::reset()
{
    m_currentDraw.color = Qt::black;
    m_currentDraw.path.clear();
    m_currentDraw.shape = NONE;
}
