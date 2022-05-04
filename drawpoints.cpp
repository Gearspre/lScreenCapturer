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
    this->resetAll();
}

void drawPointSingleton::appendPoint(const QPoint &point)
{
    m_currentDraw.path.append(point);
}

void drawPointSingleton::setCurrentAction(drawPointSingleton::drawAction action)
{
    m_currentDraw.action = action;
}

void drawPointSingleton::setCurrentSetting(const drawSetting &setting)
{
    m_currentDraw.setting.shape = setting.shape;
    m_currentDraw.setting.color = setting.color;
    m_currentDraw.setting.lineWidth = setting.lineWidth;
}

void drawPointSingleton::appendHistory()
{
    m_drawList.append(m_currentDraw);
}

void drawPointSingleton::resetAll()
{
    m_currentDraw.action = CREATE;
    m_currentDraw.setting.color = Qt::black;
    m_currentDraw.setting.lineWidth = 1;
    m_currentDraw.setting.shape = drawSetting::NONE;
    m_currentDraw.path.clear();
    m_drawList.clear();
}

void drawPointSingleton::resetPath()
{
    m_currentDraw.path.clear();
}
