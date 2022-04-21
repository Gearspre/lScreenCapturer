#ifndef LSCREENCLIPPER_H
#define LSCREENCLIPPER_H

#include <QWidget>
#include <QPixmap>
#include <QRect>

#include "drawpoints.h"

#define drapRectWidth 8
#define drapRectHeight 8

class LScreenClipper : public QWidget
{
    Q_OBJECT
    enum areaPos{
        OUTSIDE,
        INSIDE,

        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        BOTTOM_MIDDLE,

        TOP_LEFT,
        TOP_RIGHT,
        TOP_MIDDLE,

        LEFT_MIDDLE,
        RIGHT_MIDDLE
    };

    struct clipArea{

        qint32 x = 0;   /** left top x */
        qint32 y = 0;   /** left top y */
        qint32 rbx = 0; /** right bottom x */
        qint32 rby = 0; /** right bottom y */
        qint32 width = 0;
        qint32 height = 0;
        QImage img;     /** clip img */
    };

public:
    explicit LScreenClipper(QWidget *parent = nullptr);

public:
    drawPointSingleton::drawAction drawAction(const QPoint& point);
    QRect clipRect();
    QImage clipImage(bool& isVaild);

    QPoint clipTopLeft();
    QPoint clipBottomLeft();
    QPoint clipTopRight();
    QPoint clipBottomRight();

    QRect clipTopLeftRect();
    QRect clipBottomLeftRect();
    QRect clipTopRightRect();
    QRect clipBottomRightRect();

private:
    void drawAroundRect(QPainter& painter);
    bool calculateClipArea(const QPoint& first,
                           const QPoint& last,
                           clipArea& area);
    void moveClipArea(const QPoint& first,
                      const QPoint& last,
                      clipArea& area);

private:
    void screenInit();
    void clipAreaInit();
    void posHashInit();
    void dpRectInit();

protected:
    void paintEvent(QPaintEvent *event);

private:
    drawPointSingleton* m_drawPoints = nullptr;
    clipArea m_clipArea;
    QImage m_screenShot;
    QHash<areaPos, drawPointSingleton::drawAction> m_posHash;

private:
    const qint32 dpRectWidth = drapRectWidth;
    const qint32 dpRectHeight = drapRectHeight;
    const QPoint dpRectOffsetPoint =  QPoint(dpRectWidth / 2, dpRectHeight / 2);
    const QSize dpRectSize = QSize(dpRectWidth,dpRectHeight);
};

#endif // LSCREENCLIPPER_H
