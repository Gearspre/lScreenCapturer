#ifndef LSCREENPAINTER_H
#define LSCREENPAINTER_H

#include <QObject>
#include <QWidget>
#include <QMetaType>

#include "lscreenpaintertoolbar.h"

class QMouseEvent;
class QPaintEvent;

class LScreenPainter : public QWidget
{
    Q_OBJECT

public:

    struct drawSetting{
        LScreenPainterToolBar::drawShape shape;
        qint32 lineWidth;
        QColor color;
    };

    struct drawParam{
        QVector<QPoint> path;
        drawSetting setting;
    };

public:
    explicit LScreenPainter(QWidget *parent = nullptr);

public:
    void startPaint(const QPoint& topLeft, const QSize& imgSize);
    void stopPaint();

public slots:
    void onDrawSettingChanged(const drawSetting& setting);

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);

private:
    void drawInit();
    void reset();
    void drawShapes(QPainter &painter, const drawParam &dParam);

private:
    drawParam m_currentDraw;
    QList<drawParam> m_drawList;

signals:

};

Q_DECLARE_METATYPE(LScreenPainter::drawSetting)
#endif // LSCREENPAINTER_H
