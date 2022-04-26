#ifndef LSCREENCAPUTRERUI_H
#define LSCREENCAPUTRERUI_H

#include <QWidget>

#include "lscreenpainter.h"
#include "lscreenclipper.h"
#include "lscreenpaintertoolbar.h"

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
    LScreenPainter* m_painter = nullptr;
    LScreenClipper* m_clipper = nullptr;
    LScreenPainterToolBar* m_ptoolbar = nullptr;
    drawPointSingleton* m_drawPoints = nullptr;
};
#endif // LSCREENCAPUTRERUI_H
