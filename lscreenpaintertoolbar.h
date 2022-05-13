#ifndef LSCREENPAINTERTOOLBAR_H
#define LSCREENPAINTERTOOLBAR_H

#include <QToolBar>
#include <QPair>
#include "drawsetting.h"


class QMouseEvent;
class QPushButton;

class LScreenPainterToolBar : public QToolBar
{
    Q_OBJECT
    typedef void (LScreenPainterToolBar::*checkedChangeSLOT)(bool);

public:
    explicit LScreenPainterToolBar(QWidget *parent = nullptr);

public:
    bool isPainting();
    drawSetting currentSetting();
    void resetSetting();

signals:
    void settingChanged(const drawSetting& setting);
    void sigSave();
    void sigClose();
    void sigReturn();

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    void iconButtonInit();
    void toolBarInit();

private:
    void setButtonStyle(QPushButton *button,
                        const QString& imgsrc,
                        drawSetting::drawShape shape = drawSetting::NONE,
                        bool isCheck = true);

    bool addIconButton(const QString& imgSrc,
                       checkedChangeSLOT slot ,
                       drawSetting::drawShape shape = drawSetting::NONE,
                       bool isCheck = true);

    void resetOtherButton(QObject* srcBtn = nullptr);
    drawSetting::drawShape searchShape(const QObject* btn);
private slots:
    void onWriteCheckChanged(bool isCheck);
    void onSqureCheckChanged(bool isCheck);
    void onRoundCheckChanged(bool isCheck);
    void onReturnCheckChanged(bool isCheck);
    void onSaveCheckChanged(bool isCheck);
    void onCloseCheckChanged(bool isCheck);

private:
    QPoint m_lastPoint;
    QVector<QPair<QObject*, drawSetting::drawShape>> m_btnVector;
    drawSetting m_setting;
    const QString m_iconButtonStyle = "QPushButton{image: url(%1); border:0; padding: 5px}\
                                       QPushButton:hover{background-color:rgba(181, 181, 181, 25%) ;}\
                                       QPushButton:pressed{background-color:rgba(181, 181, 181, 50%);}\
                                       QPushButton:checked{background-color:rgba(181, 181, 181, 50%);}";
};

#endif // LSCREENPAINTERTOOLBAR_H
