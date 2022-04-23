#include "lscreenpainter.h"

#include <QDebug>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

LScreenPainter::LScreenPainter(QWidget *parent) : QWidget(parent)
{
    drawInit();
    this->resize(parent->size());
}

void LScreenPainter::drawInit()
{
    m_currentDraw.color = Qt::black;
    m_currentDraw.path.clear();
    m_currentDraw.shape = ELLIPSE;
}

void LScreenPainter::mousePressEvent(QMouseEvent *e)
{
    m_currentDraw.path.append(e->pos());
    qDebug()<< e->pos();
}

void LScreenPainter::mouseMoveEvent(QMouseEvent *e)
{
    m_currentDraw.path.append(e->pos());

    repaint();
}

void LScreenPainter::mouseReleaseEvent(QMouseEvent *e)
{
    m_currentDraw.path.append(e->pos());
    m_drawList.append(m_currentDraw);
    reset();
    repaint();
}

void LScreenPainter::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);

//    painter.drawLine(0,0, 50, 50);
    drawShape(painter, m_currentDraw);

    for(const drawParam& dParam : m_drawList){
        drawShape(painter, dParam);
    }
}

void LScreenPainter::reset()
{
    m_currentDraw.color = Qt::black;
    m_currentDraw.path.clear();
//    m_currentDraw.shape = NONE;
}

void LScreenPainter::drawShape(QPainter &painter, const drawParam &dParam)
{
    if(NONE == dParam.shape) return;

    if(dParam.path.count() >= 2){
        const QPoint start = dParam.path.first();
        const QPoint end = dParam.path.last();
        const qint32 width = end.x() - start.x();
        const qint32 height = end.y() - start.y();
        QColor color(dParam.color);
        painter.setPen(color);

        if(FREE == dParam.shape){
            for(int i = 0; i < dParam.path.count() - 1; i++){
                painter.drawLine(dParam.path.at(i), dParam.path.at(i+1));
            }
        }
        else if(LINE == dParam.shape){
            painter.drawLine(start, end);
        }
        else if(RECTANGLE == dParam.shape){
            painter.drawRect(start.x(), start.y(), width, height);
        }
        else if(ELLIPSE == dParam.shape){
            painter.drawEllipse(start.x(), start.y(), width, height);
        }
    }

}
