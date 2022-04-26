#include "lscreenpainter.h"

#include <QDebug>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

LScreenPainter::LScreenPainter(QWidget *parent) : QWidget(parent)
{
    drawInit();
    qRegisterMetaType<LScreenPainter::drawSetting>("LScreenPainter::drawSetting&");
    this->resize(0,0);
}

void LScreenPainter::startPaint(const QPoint &topLeft, const QSize &imgSize)
{
    this->resize(imgSize);
    this->move(topLeft);
}

void LScreenPainter::stopPaint()
{
    this->resize(0,0);
}

void LScreenPainter::onDrawSettingChanged(const drawSetting &setting)
{
    m_drawList.append(m_currentDraw);
    m_currentDraw.path.clear();
    m_currentDraw.setting.color = setting.color;
    m_currentDraw.setting.shape = setting.shape;
    m_currentDraw.setting.lineWidth = setting.lineWidth;
}

void LScreenPainter::drawInit()
{
    m_currentDraw.setting.color = QColor(Qt::black);
    m_currentDraw.path.clear();
    m_currentDraw.setting.shape = LScreenPainterToolBar::ELLIPSE;
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
    drawShapes(painter, m_currentDraw);

    for(const drawParam& dParam : m_drawList){
        drawShapes(painter, dParam);
    }
}

void LScreenPainter::reset()
{
    m_currentDraw.setting.color = Qt::black;
    m_currentDraw.setting.shape = LScreenPainterToolBar::NONE;
    m_currentDraw.setting.lineWidth = 1;
    m_currentDraw.path.clear();
}

void LScreenPainter::drawShapes(QPainter &painter, const drawParam &dParam)
{
    if(LScreenPainterToolBar::NONE == dParam.setting.shape) return;

    if(dParam.path.count() >= 2){
        const QPoint start = dParam.path.first();
        const QPoint end = dParam.path.last();
        const qint32 width = end.x() - start.x();
        const qint32 height = end.y() - start.y();
        QColor color(dParam.setting.color);
        painter.setPen(color);

        if(LScreenPainterToolBar::FREE == dParam.setting.shape){
            for(int i = 0; i < dParam.path.count() - 1; i++){
                painter.drawLine(dParam.path.at(i), dParam.path.at(i+1));
            }
        }
        else if(LScreenPainterToolBar::LINE == dParam.setting.shape){
            painter.drawLine(start, end);
        }
        else if(LScreenPainterToolBar::RECTANGLE == dParam.setting.shape){
            painter.drawRect(start.x(), start.y(), width, height);
        }
        else if(LScreenPainterToolBar::ELLIPSE == dParam.setting.shape){
            painter.drawEllipse(start.x(), start.y(), width, height);
        }
    }

}
