//#include "drawpoints.h"

#include "lScreenClipper.h"

#include <QPainter>
#include <QWindow>
#include <QScreen>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QDebug>

LScreenClipper::LScreenClipper(QWidget *parent) : QWidget(parent)
{
    this->resize(parent->size());
    screenInit();
    clipAreaInit();
    actHashInit();
    setMouseTracking(true);
    m_drawPoints = drawPointSingleton::getInstance();
}

drawPointSingleton::drawAction LScreenClipper::drawAction(const QPoint &point)
{
    drawPointSingleton::drawAction act = drawPointSingleton::CREATE;
    if(clipRect().contains(point)){
        act = drawPointSingleton::MOVE;
    }
    else if(clipTopLeftRect().contains(point)){
        act = drawPointSingleton::TOP_LEFT_EXPAND;
    }
    else if(clipTopRightRect().contains(point)){
        act = drawPointSingleton::TOP_RIGHT_EXPAND;
    }
    else if(clipBottomLeftRect().contains(point)){
        act = drawPointSingleton::BOTTOM_LEFT_EXPAND;
    }
    else if(clipBottomRightRect().contains(point)){
        act = drawPointSingleton::BOTTOM_RIGHT_EXPAND;
    }

    return act;
}

QRect LScreenClipper::clipRect()
{
    return QRect( QPoint(m_clipArea.x, m_clipArea.y) , QPoint(m_clipArea.rbx, m_clipArea.rby));
}

QSize LScreenClipper::clipSize()
{
    return QSize(m_clipArea.width, m_clipArea.height);
}

QPoint LScreenClipper::clipTopLeft()
{
    return clipRect().topLeft();
}

QRect LScreenClipper::clipTopLeftRect()
{
    return QRect(clipTopLeft() - dpRectOffsetPoint,dpRectSize);
}

QPoint LScreenClipper::clipBottomLeft()
{
    return clipRect().bottomLeft();
}

QRect LScreenClipper::clipBottomLeftRect()
{
    return QRect(clipBottomLeft() - dpRectOffsetPoint,dpRectSize);
}

QPoint LScreenClipper::clipTopRight()
{
    return clipRect().topRight();
}

QRect LScreenClipper::clipTopRightRect()
{
    return QRect(clipTopRight() - dpRectOffsetPoint,dpRectSize);
}

QPoint LScreenClipper::clipBottomRight()
{
    return clipRect().bottomRight();
}

QRect LScreenClipper::clipBottomRightRect()
{
    return QRect(clipBottomRight() - dpRectOffsetPoint,dpRectSize);
}

QPoint LScreenClipper::clipAdaptTopLeft()
{
    const qint32 topLeftX = m_clipArea.width > 0 ? m_clipArea.x : m_clipArea.x - m_clipArea.width;
    const qint32 topLeftY = m_clipArea.height > 0 ? m_clipArea.y : m_clipArea.y - m_clipArea.height;

    return QPoint(topLeftX, topLeftY);
}

QPoint LScreenClipper::clipAdaptBottomRight()
{
    const qint32 bottomLeftX = m_clipArea.width > 0 ? m_clipArea.x + m_clipArea.width : m_clipArea.x;
    const qint32 bottomLeftY = m_clipArea.height > 0 ? m_clipArea.y + m_clipArea.height : m_clipArea.y;

    return QPoint(bottomLeftX, bottomLeftY);
}

void LScreenClipper::updateClipAreaPos()
{
    m_clipPos.topLeft = clipTopLeft();
    m_clipPos.topRight = clipTopRight();
    m_clipPos.bottomLeft = clipBottomLeft();
    m_clipPos.bottomRight = clipBottomRight();
}

QImage LScreenClipper::clipImage(bool &isVaild)
{
    if(m_clipArea.img.isNull()){
        isVaild = false;
        return QImage();
    }
    else{
        isVaild = true;
        return m_clipArea.img;
    }
}

void LScreenClipper::screenInit()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    this->resize(screen->size());
    m_screenShot = screen->grabWindow(0).toImage();
//    if (!m_currentScreen)
    //        return;
}

void LScreenClipper::clipAreaInit()
{
    m_clipArea.height = 0;
    m_clipArea.width = 0;
    m_clipArea.x = 0;
    m_clipArea.y = 0;
    m_clipArea.img = QImage();
}

void LScreenClipper::actHashInit()
{
    m_actHash.insert(drawPointSingleton::TOP_RIGHT_EXPAND, &m_clipPos.bottomLeft);
    m_actHash.insert(drawPointSingleton::TOP_LEFT_EXPAND, &m_clipPos.bottomRight);
    m_actHash.insert(drawPointSingleton::BOTTOM_LEFT_EXPAND, &m_clipPos.topRight);
    m_actHash.insert(drawPointSingleton::BOTTOM_RIGHT_EXPAND, &m_clipPos.topLeft);
}

void LScreenClipper::drawAroundRect(QPainter &painter)
{
    painter.setPen(QColor(Qt::blue));
    painter.setBrush(QBrush(Qt::blue));

    painter.drawRect(clipTopLeftRect());
    painter.drawRect(clipBottomLeftRect());
    painter.drawRect(clipTopRightRect());
    painter.drawRect(clipBottomRightRect());
}

bool LScreenClipper::calculateClipArea(const QPoint &first,
                                       const QPoint &last,
                                       clipArea &area)
{
    const qint32 firstX = first.x();
    const qint32 firstY = first.y();
    const qint32 lastX = last.x();
    const qint32 lastY = last.y();

    const qint32 width = lastX - firstX;
    const qint32 height = lastY - firstY;
    const qint32 aWidth = qAbs(width);
    const qint32 aHeight = qAbs(height);

    const qint32 drawX = width > 0 ? firstX : lastX;
    const qint32 limitX = width > 0 ? lastX : firstX;
    const qint32 drawY = height > 0 ? firstY : lastY;
    const qint32 limitY = height > 0 ? lastY : firstY;

    QImage clipImg(QSize(aWidth, aHeight), QImage::Format_ARGB32);

    //row by column - left to Right
    for(qint32 clipY = drawY, srcY = 0; clipY < limitY; clipY++, srcY++){
        for(qint32 clipX = drawX, srcX = 0; clipX < limitX; clipX++,srcX++){
            clipImg.setPixel(srcX, srcY, m_screenShot.pixel(clipX, clipY));
        }
    }

    area.x = drawX;
    area.y = drawY;
    area.width = width;
    area.height = height;
    area.img = clipImg;
    area.rbx = aWidth + drawX;
    area.rby = aHeight + drawY;

    return true;
}

void LScreenClipper::moveClipArea(const QPoint &first, const QPoint &last, LScreenClipper::clipArea &area)
{
    const qint32 firstX = first.x();
    const qint32 firstY = first.y();
    const qint32 lastX = last.x();
    const qint32 lastY = last.y();

    const qint32 shiftX = lastX - firstX;
    const qint32 shiftY = lastY - firstY;
    const qint32 finallyX = area.x + shiftX;
    const qint32 finallyY = area.y + shiftY;

    const qint32 aheight = qAbs(area.height);
    const qint32 awidth = qAbs(area.width);


    for(qint32 srcY = 0; srcY < aheight; srcY++){
        qint32 offestY = srcY + finallyY;
        for(int srcX = 0; srcX < awidth; srcX++){
            qint32 offestX = srcX + finallyX;
            area.img.setPixel(srcX, srcY, m_screenShot.pixel(offestX, offestY));
        }
    }

    area.x = finallyX ;
    area.y = finallyY ;
    area.rbx = area.rbx + shiftX;
    area.rby = area.rby + shiftY;
}

void LScreenClipper::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0, 0, 0, 50)));

    painter.drawImage(0,0,m_screenShot);
    painter.drawRect(rect());

    if(m_drawPoints->current().path.count() >= 2){

        if(m_drawPoints->current().action == drawPointSingleton::MOVE){
            moveClipArea(m_drawPoints->current().path.at(m_drawPoints->current().path.count()-2),
                         m_drawPoints->current().path.last(),
                         m_clipArea);
        }

        else if(m_drawPoints->current().action == drawPointSingleton::CREATE){
            calculateClipArea(m_drawPoints->current().path.first(),
                              m_drawPoints->current().path.last(),
                              m_clipArea);
        }

        else{
            QPoint* startPoint = m_actHash.value(m_drawPoints->current().action);
            switch (m_drawPoints->current().action) {
            case drawPointSingleton::BOTTOM_RIGHT_EXPAND:
            case drawPointSingleton::BOTTOM_LEFT_EXPAND:
            case drawPointSingleton::TOP_LEFT_EXPAND:
            case drawPointSingleton::TOP_RIGHT_EXPAND:
                calculateClipArea(*startPoint,
                                  m_drawPoints->current().path.last(),
                                  m_clipArea);
            default:
                break;
            }
        }

        if(m_clipArea.width && m_clipArea.height){
            painter.drawImage(QRect(m_clipArea.x, m_clipArea.y, m_clipArea.width, m_clipArea.height), m_clipArea.img);
            drawAroundRect(painter);
        }
    }

    else if(m_drawPoints->history().count() > 0){

        if(m_drawPoints->history().last().action == drawPointSingleton::CREATE){
            calculateClipArea(m_drawPoints->history().last().path.first(),
                              m_drawPoints->history().last().path.last(),
                              m_clipArea);
        }

        else if(m_drawPoints->history().last().action == drawPointSingleton::MOVE){
            calculateClipArea(clipAdaptTopLeft(),
                              clipAdaptBottomRight(),
                              m_clipArea);
        }

        if(m_clipArea.width && m_clipArea.height){
            painter.drawImage(QRect(m_clipArea.x, m_clipArea.y, m_clipArea.width, m_clipArea.height), m_clipArea.img);
            drawAroundRect(painter);
            updateClipAreaPos();
        }
    }
}
