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
    return QRect(m_clipArea.x,m_clipArea.y, m_clipArea.width, m_clipArea.height);
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
    const qint32 drawX = width > 0 ? firstX : lastX;
    const qint32 limitX = width > 0 ? lastX : firstX;
    const qint32 drawY = height > 0 ? firstY : lastY;
    const qint32 limitY = height > 0 ? lastY : firstY;
    const qint32 aWidth = qAbs(width);
    const qint32 aHeight = qAbs(height);

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

void LScreenClipper::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0, 0, 0, 50)));

    painter.drawImage(0,0,m_screenShot);
    painter.drawRect(rect());

    if(m_drawPoints->current().path.count() >= 2){
        calculateClipArea(m_drawPoints->current().path.first(),
                          m_drawPoints->current().path.last(),
                          m_clipArea);

        if(m_clipArea.width && m_clipArea.height){
            painter.drawImage(clipRect(), m_clipArea.img);
        }
    }

    else if(m_drawPoints->history().count() > 0){
        calculateClipArea(m_drawPoints->history().last().path.first(),
                          m_drawPoints->history().last().path.last(),
                          m_clipArea);

        if(m_clipArea.width && m_clipArea.height){
            painter.drawImage(clipRect(), m_clipArea.img);
        }
    }
}

//void LScreenClipper::mouseMoveEvent(QMouseEvent *event)
//{
//    if( QRect(m_clipArea.x,m_clipArea.y, m_clipArea.width, m_clipArea.height).contains(event->pos()) ){
//        this->setCursor(Qt::SizeAllCursor);
//    }
//    else{
//        this->unsetCursor();
//    }
//    repaint();
//}
