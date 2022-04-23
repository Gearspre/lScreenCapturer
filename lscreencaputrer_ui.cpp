#include "lscreencaputrer_ui.h"
#include "ui_lscreencaputrer_ui.h"

#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QWindow>
#include <QImage>
#include <QDebug>
#include <QStyle>

#include "drawpoints.h"

LScreenCaputrerUI::LScreenCaputrerUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LScreenCaputrerUI)
{
    ui->setupUi(this);
    resize(600, 400);
    setMouseTracking(true);
    m_drawPoints = m_drawPoints->getInstance();

    toolbar = new LScreenPainterToolBar();
    toolbar->show();

    //just test
//    test1 = new LScreenClipper(this);
//    test1->setAttribute(Qt::WA_TransparentForMouseEvents, true);

//    QPushButton* testSave = new QPushButton("save",this);
//    connect(testSave, &QPushButton::clicked, [this](){
//        bool isVaild = false;
//        QImage img = test1->clipImage(isVaild);
//        if(isVaild)
//        img.save("D://test.png", "PNG");
//    });

//    test = new BasePainter(this);
}

LScreenCaputrerUI::~LScreenCaputrerUI()
{
    delete ui;
    delete toolbar;
}

void LScreenCaputrerUI::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        drawPointSingleton::drawAction act = test1->drawAction(e->pos());
        m_drawPoints->appendPoint(e->pos());
        m_drawPoints->setCurrentAction(act);
    }
}

void LScreenCaputrerUI::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        m_drawPoints->appendPoint(e->pos());
    }
    else{
        drawPointSingleton::drawAction act = test1->drawAction(e->pos());

        if( drawPointSingleton::CREATE == act){
            setCursor(Qt::CrossCursor);
        }
        else if( drawPointSingleton::MOVE == act){
            setCursor(Qt::SizeAllCursor);
        }
        else if( drawPointSingleton::TOP_LEFT_EXPAND == act){
            setCursor(Qt::SizeFDiagCursor);
        }
        else if( drawPointSingleton::TOP_RIGHT_EXPAND == act){
            setCursor(Qt::SizeBDiagCursor);
        }
        else if( drawPointSingleton::BOTTOM_LEFT_EXPAND == act){
            setCursor(Qt::SizeBDiagCursor);
        }
        else if( drawPointSingleton::BOTTOM_RIGHT_EXPAND == act){
            setCursor(Qt::SizeFDiagCursor);
        }
        else{
            unsetCursor();
        }
//        if(test1->clipRect().contains(e->pos())){

//        }

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




