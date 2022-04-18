#ifndef LSCREENCLIPPER_H
#define LSCREENCLIPPER_H

#include <QWidget>
#include <QPixmap>
#include <QRect>

class QMouseEvent;
class drawPointSingleton;

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

private:
    void screenInit();
    void clipAreaInit();
    bool calculateClipArea(const QPoint& first,
                           const QPoint& last,
                           clipArea& area);

protected:
    void paintEvent(QPaintEvent *event);
//    void mouseMoveEvent(QMouseEvent* event);

private:
    drawPointSingleton* m_drawPoints = nullptr;
    clipArea m_clipArea;
    QImage m_screenShot;
    QPixmap m_screenShotBackground;
};

#endif // LSCREENCLIPPER_H
