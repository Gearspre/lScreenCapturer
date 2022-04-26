#ifndef LSCREENPAINTERTOOLBAR_H
#define LSCREENPAINTERTOOLBAR_H

#include <QToolBar>
#include <QPair>


class QMouseEvent;
class QPushButton;

class LScreenPainterToolBar : public QToolBar
{
    Q_OBJECT
    typedef void (LScreenPainterToolBar::*checkedChangeSLOT)(bool);

public:
    enum drawShape{
        NONE,
        FREE,
        LINE,
        RECTANGLE,
        ELLIPSE
    };

public:
    explicit LScreenPainterToolBar(QWidget *parent = nullptr);

public:
    drawShape currentShape();

signals:
    void activePainter(drawShape);

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    void iconButtonInit();
    void toolBarInit();

private:
    void setButtonStyle(QPushButton *button, const QString& imgsrc, drawShape shape = NONE, bool isCheck = true);
    bool addIconButton(const QString& imgSrc, checkedChangeSLOT slot , drawShape shape = NONE, bool isCheck = true);
    void resetOtherButton(QObject* srcBtn);
    drawShape searchShape(const QObject* btn);
private slots:
    void onWriteCheckChanged(bool isCheck);
    void onSqureCheckChanged(bool isCheck);
    void onRoundCheckChanged(bool isCheck);
    void onReturnCheckChanged(bool isCheck);
    void onSaveCheckChanged(bool isCheck);

private:
    QPoint m_lastPoint;
    QVector<QPair<QObject*, drawShape>> m_btnVector;
    const QString m_iconButtonStyle = "QPushButton{image: url(%1); border:0; padding: 5px}\
                                       QPushButton:hover{background-color:rgba(181, 181, 181, 25%) ;}\
                                       QPushButton:pressed{background-color:rgba(181, 181, 181, 50%);}\
                                       QPushButton:checked{background-color:rgba(181, 181, 181, 50%);}";
};

#endif // LSCREENPAINTERTOOLBAR_H
