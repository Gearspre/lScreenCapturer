#ifndef LSCREENPAINTER_H
#define LSCREENPAINTER_H

#include <QObject>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

class LScreenPainter : public QWidget
{
    Q_OBJECT
private:
    enum drawShape{
        NONE,
        FREE,
        LINE,
        RECTANGLE,
        ELLIPSE
    };
    struct drawParam{
        drawShape shape;
        QVector<QPoint> path;
        Qt::GlobalColor color;
    };

public:
    explicit LScreenPainter(QWidget *parent = nullptr);

public:


protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);

private:
    void drawInit();
    void reset();
    void drawShape(QPainter &painter, const drawParam &dParam);

private:
    drawParam m_currentDraw;
    QList<drawParam> m_drawList;

signals:

};

#endif // LSCREENPAINTER_H
