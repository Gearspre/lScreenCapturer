#ifndef LSCREENCLIPPER_H
#define LSCREENCLIPPER_H

#include <QWidget>
#include <QPixmap>
#include <QRect>

class QMouseEvent;
class drawPointSingleton;

#define drapRectWidth 4
#define drapRectHeight 4

class LScreenClipper : public QWidget
{
    Q_OBJECT
    struct clipArea{
        qint32 x = 0;
        qint32 y = 0;
        qint32 width = 0;
        qint32 height = 0;
        QImage img;
    };

public:
    explicit LScreenClipper(QWidget *parent = nullptr);

public:
    QRect clipRect();
    QPoint clipLeftTop();
    QPoint clipLeftBottom();
    QPoint clipRightTop();
    QPoint clipRightBottom();
    QImage clipImage(bool& isVaild);

private:
    void screenInit();
    void clipAreaInit();
    void drawAroundRect(QPainter& painter);
    bool calculateClipArea(const QPoint& first,
                           const QPoint& last,
                           clipArea& area);
    void moveClipArea(const QPoint& first,
                      const QPoint& last,
                      clipArea& area);

protected:
    void paintEvent(QPaintEvent *event);

private:
    drawPointSingleton* m_drawPoints = nullptr;
    clipArea m_clipArea;
    QImage m_screenShot;

private:
    const qint32 dpRectWidth = drapRectWidth;
    const qint32 dpRectHeight = drapRectHeight;
};

#endif // LSCREENCLIPPER_H
