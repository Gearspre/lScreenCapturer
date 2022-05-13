#include "lscreenpainter.h"

#include <QDebug>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

LScreenPainter::LScreenPainter(QWidget *parent) : QWidget(parent)
{
    drawInit();
    this->resize(0,0);
    setMouseTracking(true);
}

void LScreenPainter::startPaint(const QPoint &topLeft, const QImage &img)
{
    this->resize(img.size());
    this->move(topLeft);
    m_background = img;
    m_output = m_background.copy();
    m_topleft = topLeft;
}

void LScreenPainter::stopPaint()
{
    this->resize(0,0);
}

QImage LScreenPainter::getImage()
{
    return m_output;
}

QImage LScreenPainter::getOriginImage()
{
    return m_background;
}

void LScreenPainter::drawInit()
{
    m_drawPoint = drawPointSingleton::getInstance();
}

void LScreenPainter::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter imgPainter(&m_output);
    QPainter painter(this);
    painter.drawImage(0,0,m_background);
    if(m_drawPoint->current().action == drawPointSingleton::PAINTING){
        painter.setPen(QPen(Qt::blue, 3, Qt::DashLine));
        painter.drawRect(rect() - QMargins(0,0,1,1));
    }
    drawShapes(painter, m_drawPoint->current());
    drawShapes(imgPainter, m_drawPoint->current());
    for(const drawPointSingleton::drawParam& dParam : m_drawPoint->history()){
        drawShapes(painter, dParam);
        drawShapes(imgPainter, dParam);
    }
}

void LScreenPainter::drawShapes(QPainter &painter, const drawPointSingleton::drawParam &dParam)
{
    if(drawSetting::NONE != m_drawPoint->current().setting.shape){
        if(dParam.path.count() >= 2){
            const QPoint start = dParam.path.first() - m_topleft;
            const QPoint end = dParam.path.last() - m_topleft;
            const qint32 width = end.x() - start.x();
            const qint32 height = end.y() - start.y();
            QColor color(dParam.setting.color);
            painter.setPen(color);

            if(drawSetting::FREE == dParam.setting.shape){
                for(int i = 0; i < dParam.path.count() - 1; i++){
                    painter.drawLine(dParam.path.at(i) - m_topleft, dParam.path.at(i+1) - m_topleft);
                }
            }
            else if(drawSetting::LINE == dParam.setting.shape){
                painter.drawLine(start, end);
            }
            else if(drawSetting::RECTANGLE == dParam.setting.shape){
                painter.drawRect(start.x(), start.y(), width, height);
            }
            else if(drawSetting::ELLIPSE == dParam.setting.shape){
                painter.drawEllipse(start.x(), start.y(), width, height);
            }
        }
    }


}
