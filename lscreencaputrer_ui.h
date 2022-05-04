#ifndef LSCREENCAPUTRERUI_H
#define LSCREENCAPUTRERUI_H

#include <QWidget>
#include <QSystemTrayIcon>

#include "lkeycapturer.h"
#include "lscreenpainter.h"
#include "lscreenclipper.h"
#include "lscreenpaintertoolbar.h"

class QPaintEvent;
class QMouseEvent;
class QKeyEvent;
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
    void paintEvent(QPaintEvent* e);

private slots:
    void onDrawSettingChanged(drawSetting setting);
    void onKeyBoardPressed(int key);
    void onKeyBoardRelease(int key);
    void onSigSave();

private:
    void clipStart();
    void clipFinish();
    void clipMove();
    void clipCancel();

private:
    void screenInit();
    void systemTrayInit();
    void toolbarInit();
    void keyCaptureInit();

private:
    Ui::LScreenCaputrerUI *ui;
    LKeyCapturer* m_keyCapturer = nullptr;
    LScreenPainter* m_painter = nullptr;
    LScreenClipper* m_clipper = nullptr;
    LScreenPainterToolBar* m_ptoolbar = nullptr;
    drawPointSingleton* m_drawPoints = nullptr;

    QSystemTrayIcon* m_systemTray = nullptr;
    QImage m_background;
    QHash<int, bool> m_keyHash;

    bool m_isSleep = true;
};
#endif // LSCREENCAPUTRERUI_H
