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

    struct clipAreaPos{
        QPoint topLeft;
        QPoint topRight;
        QPoint bottomLeft;
        QPoint bottomRight;
    };

    struct clipArea{
        qint32 x = 0;   /** left top x */
        qint32 y = 0;   /** left top y */
        qint32 rbx = 0; /** right bottom x */
        qint32 rby = 0; /** right bottom y */
        qint32 offsetx = 0; /** horizontal distance between mouse and clip left top **/
        qint32 offsety = 0; /** vertical distance between mouse and clip left top **/
        qint32 width = 0;
        qint32 height = 0;
        QImage img;     /** clip img */
    };

public:
    explicit LScreenClipper(QWidget *parent = nullptr);

public:
    void reset();
    void updateClipOffset();
    void setScreenShot(QImage* img);

public:
    drawPointSingleton::drawAction drawAction(const QPoint& point);
    QRect clipRect();
    QSize clipSize();
    QImage clipImage(bool& isVaild);

    QPoint clipTopLeft();
    QPoint clipBottomLeft();
    QPoint clipTopRight();
    QPoint clipBottomRight();

    QRect clipTopLeftRect();
    QRect clipBottomLeftRect();
    QRect clipTopRightRect();
    QRect clipBottomRightRect();

    QPoint clipAdaptTopLeft(); /** always return top left point whenever width > 0 */
    QPoint clipAdaptBottomRight();

private:
    void updateClipAreaPos();
    void drawAroundRect(QPainter& painter);
    bool calculateClipArea(const QPoint& first,
                           const QPoint& last,
                           clipArea& area);
    void moveClipArea(const QPoint& last,
                      clipArea& area);

private:
    void screenInit();
    void clipAreaInit();
    void actHashInit();
    void dpRectInit();

protected:
    void paintEvent(QPaintEvent *event);

private:
    drawPointSingleton* m_drawPoints = nullptr;
    clipArea m_clipArea;
    clipAreaPos m_clipPos;/** never change before drop down mouse */
    QImage* m_screenShot = nullptr;
    QHash<drawPointSingleton::drawAction, QPoint*> m_actHash;
    bool isHide = false;

private:
    const qint32 dpRectWidth = drapRectWidth;
    const qint32 dpRectHeight = drapRectHeight;
    const QPoint dpRectOffsetPoint =  QPoint(dpRectWidth / 2, dpRectHeight / 2);
    const QSize dpRectSize = QSize(dpRectWidth,dpRectHeight);
};

#endif // LSCREENCLIPPER_H
