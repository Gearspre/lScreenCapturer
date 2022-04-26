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

LScreenPainterToolBar::drawShape LScreenPainterToolBar::currentShape()
{
    drawShape shape = NONE;
    for(auto& obj : m_btnVector){
        QPushButton* btn = qobject_cast<QPushButton*>(obj.first);
        if(btn->isChecked()){
            shape = obj.second;
            break;
        }
    }

    return shape;
}

void LScreenPainterToolBar::iconButtonInit()
{
    bool ret = addIconButton(":/icon/icon/write.svg", &LScreenPainterToolBar::onWriteCheckChanged, FREE);
    ret = ret && addIconButton(":/icon/icon/square.svg", &LScreenPainterToolBar::onSqureCheckChanged, RECTANGLE);
    ret = ret && addIconButton(":/icon/icon/round.svg", &LScreenPainterToolBar::onRoundCheckChanged, ELLIPSE);
    ret = ret && addIconButton(":/icon/icon/return.svg", &LScreenPainterToolBar::onReturnCheckChanged, NONE, false);
    ret = ret && addIconButton(":/icon/icon/save.svg", &LScreenPainterToolBar::onSaveCheckChanged, NONE, false);
}

void LScreenPainterToolBar::toolBarInit()
{
    this->setStyleSheet("background-color: #f6f6f6");
    this->setWindowFlags(Qt::Window |Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void LScreenPainterToolBar::setButtonStyle(QPushButton *button,
                                           const QString &imgsrc,
                                           drawShape shape,
                                           bool isCheck)
{
    const int img_w= ICON_WIDTH;
    const int img_h= ICON_HEIGHT;

    if(isCheck) m_btnVector.push_back(QPair<QObject*, drawShape>(button, shape));
    button->setCheckable(isCheck);
    button->setFixedSize(img_w,img_h);
    button->setStyleSheet(m_iconButtonStyle.arg(imgsrc));
}


bool LScreenPainterToolBar::addIconButton(const QString &imgSrc,
                                          checkedChangeSLOT slot,
                                          drawShape shape,
                                          bool isCheck)
{
    bool ret = false;
    QPushButton* btn = new QPushButton();

    if(btn){
        ret = true;
        setButtonStyle(btn, imgSrc, shape, isCheck);
        addWidget(btn);
        connect(btn, &QPushButton::clicked, this, slot);
    }

    return ret;
}

void LScreenPainterToolBar::resetOtherButton(QObject *srcBtn)
{
    for(auto& obj : m_btnVector){
        QPushButton* btn = qobject_cast<QPushButton*>(obj.first);
        if(btn->isChecked() && obj.first != srcBtn){
            btn->setChecked(false);
        }
    }
}

LScreenPainterToolBar::drawShape LScreenPainterToolBar::searchShape(const QObject *btn)
{
    drawShape shape = NONE;
    for(auto& obj : m_btnVector){
        if(btn == obj.first ){
            shape = obj.second;
            break;
        }
    }

    return shape;
}

void LScreenPainterToolBar::onWriteCheckChanged(bool isCheck)
{
    if(isCheck){
        resetOtherButton(sender());
        drawShape shape = searchShape(sender());
        emit activePainter(shape);
    }

}

void LScreenPainterToolBar::onSqureCheckChanged(bool isCheck)
{
    if(isCheck){
        resetOtherButton(sender());
        drawShape shape = searchShape(sender());
        emit activePainter(shape);
    }

}

void LScreenPainterToolBar::onRoundCheckChanged(bool isCheck)
{
    if(isCheck){
        resetOtherButton(sender());
        drawShape shape = searchShape(sender());
        emit activePainter(shape);
    }
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
