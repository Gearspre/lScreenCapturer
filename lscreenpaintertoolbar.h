#ifndef LSCREENPAINTERTOOLBAR_H
#define LSCREENPAINTERTOOLBAR_H

#include <QToolBar>

class QMouseEvent;
class QPushButton;

class LScreenPainterToolBar : public QToolBar
{
    Q_OBJECT
    typedef void (LScreenPainterToolBar::*checkedChangeSLOT)(bool);

public:
    explicit LScreenPainterToolBar(QWidget *parent = nullptr);

signals:

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    void iconButtonInit();
    void toolBarInit();

private:
    void setButtonStyle(QPushButton *button, const QString& imgsrc, bool isCheck = true);
    bool addIconButton(const QString& imgSrc, checkedChangeSLOT slot , bool isCheck = true);
    void resetOtherButton(QObject* srcBtn);

private slots:
    void onWriteCheckChanged(bool isCheck);
    void onSqureCheckChanged(bool isCheck);
    void onRoundCheckChanged(bool isCheck);
    void onReturnCheckChanged(bool isCheck);
    void onSaveCheckChanged(bool isCheck);

private:
    QPoint m_lastPoint;
    QVector<QObject*> m_btnVector;
    const QString m_iconButtonStyle = "QPushButton{image: url(%1); border:0; padding: 5px}\
                                       QPushButton:hover{background-color:rgba(181, 181, 181, 25%) ;}\
                                       QPushButton:pressed{background-color:rgba(181, 181, 181, 50%);}\
                                       QPushButton:checked{background-color:rgba(181, 181, 181, 50%);}";
};

#endif // LSCREENPAINTERTOOLBAR_H
