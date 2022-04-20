#include "lscreencaputrer_ui.h"
#include "ui_lscreencaputrer_ui.h"

#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QWindow>
#include <QImage>
#include <QDebug>
#include <QPushButton>

#include "drawpoints.h"

LScreenCaputrerUI::LScreenCaputrerUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LScreenCaputrerUI)
{
    ui->setupUi(this);
    resize(600, 400);
    setMouseTracking(true);
    m_drawPoints = m_drawPoints->getInstance();

    //just test
    test1 = new LScreenClipper(this);
    test1->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QPushButton* testSave = new QPushButton("save",this);
    connect(testSave, &QPushButton::clicked, [this](){
        bool isVaild = false;
        QImage img = test1->clipImage(isVaild);
        if(isVaild)
        img.save("D://test.png", "PNG");
    });
//    test = new BasePainter(this);
}

LScreenCaputrerUI::~LScreenCaputrerUI()
{
    delete ui;
}

void LScreenCaputrerUI::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        m_drawPoints->appendPoint(e->pos());
        if(test1->clipRect().contains(e->pos())){
            m_drawPoints->setCurrentAction(drawPointSingleton::MOVE);
        }
        else{
            m_drawPoints->setCurrentAction(drawPointSingleton::CREATE);
        }
    }
}

void LScreenCaputrerUI::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        m_drawPoints->appendPoint(e->pos());
    }
    else{
        if(test1->clipRect().contains(e->pos())){
            setCursor(Qt::SizeAllCursor);
        }
        else{
            unsetCursor();
        }
    }

    repaint();
}

void LScreenCaputrerUI::mouseReleaseEvent(QMouseEvent *e)
{
    m_drawPoints->appendPoint(e->pos());
    m_drawPoints->appendHistory();
    m_drawPoints->reset();
    repaint();
}




