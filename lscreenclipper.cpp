#include "lScreenClipper.h"

#include <QPainter>
#include <QWindow>
#include <QScreen>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QDebug>

#include "drawpoints.h"

LScreenClipper::LScreenClipper(QWidget *parent) : QWidget(parent)
{
    this->resize(parent->size());
    screenInit();
    clipAreaInit();
    setMouseTracking(true);
    m_drawPoints = drawPointSingleton::getInstance();
}

QRect LScreenClipper::clipRect()
{
    return QRect(m_clipArea.x, m_clipArea.y, m_clipArea.width, m_clipArea.height);
}

QPoint LScreenClipper::clipLeftTop()
{
    return QPoint(m_clipArea.x, m_clipArea.y);
}

QPoint LScreenClipper::clipLeftBottom()
{
    return QPoint(m_clipArea.x, m_clipArea.y + m_clipArea.height);
}

QPoint LScreenClipper::clipRightTop()
{
    return QPoint(m_clipArea.x + m_clipArea.width, m_clipArea.y);
}

QPoint LScreenClipper::clipRightBottom()
{
    return QPoint(m_clipArea.x + m_clipArea.width, m_clipArea.y + m_clipArea.height);
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

void LScreenClipper::drawAroundRect(QPainter &painter)
{
    painter.setPen(QColor(Qt::blue));
    painter.setBrush(QBrush(Qt::blue));
    QPoint offsetPoint = QPoint(dpRectWidth / 2, dpRectHeight / 2);
    QSize rectSize = QSize(dpRectWidth,dpRectHeight);

    painter.drawRect(QRect(clipLeftTop() - offsetPoint,rectSize));
    painter.drawRect(QRect(clipRightTop() - offsetPoint,rectSize));
    painter.drawRect(QRect(clipLeftBottom() - offsetPoint,rectSize));
    painter.drawRect(QRect(clipRightBottom() - offsetPoint,rectSize));
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
            painter.drawImage(clipRect(), m_clipArea.img);
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
            calculateClipArea(QPoint(m_clipArea.x, m_clipArea.y),
                              QPoint(m_clipArea.x + m_clipArea.width, m_clipArea.y + m_clipArea.height),
                              m_clipArea);
        }

        if(m_clipArea.width && m_clipArea.height){
            painter.drawImage(clipRect(), m_clipArea.img);
            drawAroundRect(painter);
        }
    }
}
