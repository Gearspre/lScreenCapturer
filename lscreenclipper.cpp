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
    posHashInit();
    clipAreaInit();
    setMouseTracking(true);
    m_drawPoints = drawPointSingleton::getInstance();
}

drawPointSingleton::drawAction LScreenClipper::drawAction(const QPoint &point)
{
    areaPos apos = OUTSIDE;
    if(clipRect().contains(point)){
        apos = INSIDE;
    }
    else if(clipTopLeftRect().contains(point)){
        apos = TOP_LEFT;
    }
    else if(clipTopRightRect().contains(point)){
        apos = TOP_RIGHT;
    }
    else if(clipBottomLeftRect().contains(point)){
        apos = BOTTOM_LEFT;
    }
    else if(clipBottomRightRect().contains(point)){
        apos = BOTTOM_RIGHT;
    }


    return m_posHash.value(apos);
}

QRect LScreenClipper::clipRect()
{
    return QRect( QPoint(m_clipArea.x, m_clipArea.y) , QPoint(m_clipArea.rbx, m_clipArea.rby));
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

void LScreenClipper::posHashInit()
{
    m_posHash.insert(OUTSIDE, drawPointSingleton::CREATE);
    m_posHash.insert(INSIDE, drawPointSingleton::MOVE);

    m_posHash.insert(BOTTOM_LEFT,  drawPointSingleton::BOTTOM_LEFT_EXPAND);
    m_posHash.insert(BOTTOM_RIGHT, drawPointSingleton::BOTTOM_RIGHT_EXPAND);
    m_posHash.insert(BOTTOM_MIDDLE, drawPointSingleton::BOTTOM_EXPAND);

    m_posHash.insert(TOP_LEFT,  drawPointSingleton::TOP_LEFT_EXPAND);
    m_posHash.insert(TOP_RIGHT, drawPointSingleton::TOP_RIGHT_EXPAND);
    m_posHash.insert(TOP_MIDDLE, drawPointSingleton::TOP_EXPAND);

    m_posHash.insert(LEFT_MIDDLE, drawPointSingleton::LEFT_EXPAND);
    m_posHash.insert(RIGHT_MIDDLE, drawPointSingleton::RIGHT_EXPAND);
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
            qint32 startX = 0;
            qint32 startY = 0;
            qint32 endX = 0;
            qint32 endY = 0;

            // reset start and end
            if(m_clipArea.width > 0){
                startX = m_clipArea.x;
                endX = m_clipArea.x + m_clipArea.width;
            }
            else{
                startX = m_clipArea.x - m_clipArea.width;
                endX = m_clipArea.x;
            }

            if(m_clipArea.height > 0){
                startY = m_clipArea.y;
                endY = m_clipArea.y + m_clipArea.height;
            }
            else{
                startY = m_clipArea.y - m_clipArea.height;
                endY = m_clipArea.y;
            }

            calculateClipArea(QPoint(startX, startY),
                              QPoint(endX, endY),
                              m_clipArea);
        }

        if(m_clipArea.width && m_clipArea.height){
            painter.drawImage(QRect(m_clipArea.x, m_clipArea.y, m_clipArea.width, m_clipArea.height), m_clipArea.img);
            drawAroundRect(painter);
        }
    }
}
