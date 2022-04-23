#include "lscreenpaintertoolbar.h"

#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>
#include <QAction>

#define ICON_WIDTH 40
#define ICON_HEIGHT 40

LScreenPainterToolBar::LScreenPainterToolBar(QWidget *parent) : QToolBar(parent)
{
    toolBarInit();
    iconButtonInit();
}

void LScreenPainterToolBar::iconButtonInit()
{
    bool ret = addIconButton(":/icon/icon/write.svg", &LScreenPainterToolBar::onWriteCheckChanged);
    ret = ret && addIconButton(":/icon/icon/square.svg", &LScreenPainterToolBar::onSqureCheckChanged);
    ret = ret && addIconButton(":/icon/icon/round.svg", &LScreenPainterToolBar::onRoundCheckChanged);
    ret = ret && addIconButton(":/icon/icon/return.svg", &LScreenPainterToolBar::onReturnCheckChanged, false);
    ret = ret && addIconButton(":/icon/icon/save.svg", &LScreenPainterToolBar::onSaveCheckChanged, false);
}

void LScreenPainterToolBar::toolBarInit()
{
    this->setStyleSheet("background-color: #f6f6f6");
    this->setWindowFlags(Qt::Window |Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void LScreenPainterToolBar::setButtonStyle(QPushButton *button, const QString &imgsrc, bool isCheck)
{
    const int img_w= ICON_WIDTH;
    const int img_h= ICON_HEIGHT;

    if(isCheck) m_btnVector.push_back(button);
    button->setCheckable(isCheck);
    button->setFixedSize(img_w,img_h);
    button->setStyleSheet(m_iconButtonStyle.arg(imgsrc));
}


bool LScreenPainterToolBar::addIconButton(const QString &imgSrc,checkedChangeSLOT slot, bool isCheck)
{
    bool ret = false;
    QPushButton* btn = new QPushButton();

    if(btn){
        ret = true;
        setButtonStyle(btn, imgSrc, isCheck);
        addWidget(btn);
        connect(btn, &QPushButton::clicked, this, slot);
    }

    return ret;
}

void LScreenPainterToolBar::resetOtherButton(QObject *srcBtn)
{
    for(QObject* obj : m_btnVector){
        QPushButton* btn = qobject_cast<QPushButton*>(obj);
        if(btn->isChecked() && obj != srcBtn){
            btn->setChecked(false);
        }
    }
}

void LScreenPainterToolBar::onWriteCheckChanged(bool isCheck)
{
    if(isCheck)
        resetOtherButton(sender());
}

void LScreenPainterToolBar::onSqureCheckChanged(bool isCheck)
{
    if(isCheck)
        resetOtherButton(sender());
}

void LScreenPainterToolBar::onRoundCheckChanged(bool isCheck)
{
    if(isCheck)
        resetOtherButton(sender());
}

void LScreenPainterToolBar::onReturnCheckChanged(bool isCheck)
{
    Q_UNUSED(isCheck)

    resetOtherButton(sender());
}

void LScreenPainterToolBar::onSaveCheckChanged(bool isCheck)
{
    Q_UNUSED(isCheck)

    resetOtherButton(sender());
}

void LScreenPainterToolBar::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        m_lastPoint = e->globalPos();
    }
}

void LScreenPainterToolBar::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() & Qt::LeftButton) && !actionAt(e->pos())){
        const QPoint position = pos() + e->globalPos() - m_lastPoint;
        move(position.x(),position.y());
        m_lastPoint = e->globalPos();
    }
}
