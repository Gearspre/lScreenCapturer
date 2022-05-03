#include "lscreenpainter.h"

#include <QDebug>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

LScreenPainter::LScreenPainter(QWidget *parent) : QWidget(parent)
{
    drawInit();
//    qRegisterMetaType<LScreenPainter::drawSetting>("LScreenPainter::drawSetting&");
    this->resize(0,0);
    setMouseTracking(true);
}

void LScreenPainter::startPaint(const QPoint &topLeft, const QImage &img)
{
    this->resize(img.size());
    this->move(topLeft);
    m_background = img;
    m_topleft = topLeft;
}

void LScreenPainter::stopPaint()
{
    this->resize(0,0);
}

void LScreenPainter::drawInit()
{
    m_drawPoint = drawPointSingleton::getInstance();
}

void LScreenPainter::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    painter.drawImage(0,0,m_background);
    drawShapes(painter, m_drawPoint->current());

    for(const drawPointSingleton::drawParam& dParam : m_drawPoint->history()){
        drawShapes(painter, dParam);
    }
}

void LScreenPainter::drawShapes(QPainter &painter, const drawPointSingleton::drawParam &dParam)
{
    if(drawSetting::NONE != m_drawPoint->current().setting.shape){
        if(dParam.path.count() >= 2){
//            const QPoint offset = QPoint(m_background.width(), m_background.height());
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
//                qDebug() << "start.x(), start.y()" <<start.x()<< start.y();
            }
            else if(drawSetting::ELLIPSE == dParam.setting.shape){
                painter.drawEllipse(start.x(), start.y(), width, height);
            }
        }
    }


}
