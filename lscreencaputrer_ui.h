#ifndef LSCREENCAPUTRERUI_H
#define LSCREENCAPUTRERUI_H

#include <QWidget>

#include "basepainter.h"
#include "lscreenclipper.h"

class QPaintEvent;
class QMouseEvent;
class QScreen;
class drawPointSingleton;
class LScreenClipper;

QT_BEGIN_NAMESPACE
namespace Ui { class LScreenCaputrerUI; }
QT_END_NAMESPACE

class LScreenCaputrerUI : public QWidget
{
    Q_OBJECT

public:
    LScreenCaputrerUI(QWidget *parent = nullptr);
    ~LScreenCaputrerUI();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    Ui::LScreenCaputrerUI *ui;
    BasePainter* test;
    LScreenClipper* test1;
    drawPointSingleton* m_drawPoints = nullptr;
};
#endif // LSCREENCAPUTRERUI_H
