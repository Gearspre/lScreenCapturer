#ifndef LSCREENPAINTER_H
#define LSCREENPAINTER_H

#include <QObject>
#include <QWidget>
#include <QMetaType>

#include "drawpoints.h"

class QMouseEvent;
class QPaintEvent;

class LScreenPainter : public QWidget
{
    Q_OBJECT

public:
    explicit LScreenPainter(QWidget *parent = nullptr);

public:
    void startPaint(const QPoint& topLeft, const QImage& img);
    void stopPaint();

protected:
    void paintEvent(QPaintEvent* e);

private:
    void drawInit();
    void reset();
    void drawShapes(QPainter &painter, const drawPointSingleton::drawParam& dParam);

private:
    drawPointSingleton* m_drawPoint = nullptr;
    QImage m_background;
    QPoint m_topleft;
signals:

};


#endif // LSCREENPAINTER_H
