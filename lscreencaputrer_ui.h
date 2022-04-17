#ifndef LSCREENCAPUTRERUI_H
#define LSCREENCAPUTRERUI_H

#include <QWidget>

#include "basepainter.h"

class QPaintEvent;
class QMouseEvent;
class QScreen;

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

private:
    void screenInit();
//    void reset();
//    void drawShape(QPainter& painter, const drawParam& dParam);

private:
    QVector<QPoint> m_path;
    Ui::LScreenCaputrerUI *ui;
    BasePainter* test;
    QScreen* m_currentScreen = nullptr;
    QPixmap m_pmap;
    QImage m_srcMap;
};
#endif // LSCREENCAPUTRERUI_H
