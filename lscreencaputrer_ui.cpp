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
    , m_clipper(new LScreenClipper(this))
    , m_painter(new LScreenPainter(this))
    , m_ptoolbar(new LScreenPainterToolBar())
    , m_drawPoints(m_drawPoints->getInstance())
{
    ui->setupUi(this);
    screenInit();

    connect(m_ptoolbar, SIGNAL(settingChanged(drawSetting)), this, SLOT(onDrawSettingChanged(drawSetting)));

    m_painter->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_clipper->setAttribute(Qt::WA_TransparentForMouseEvents, true);

//    QPushButton* testSave = new QPushButton("save",this);
//    connect(testSave, &QPushButton::clicked, [this](){
//        bool isVaild = false;
//        QImage img = test1->clipImage(isVaild);
//        if(isVaild)
//        img.save("D://test.png", "PNG");
//    });

}

LScreenCaputrerUI::~LScreenCaputrerUI()
{
    delete ui;
    delete m_ptoolbar;
}

void LScreenCaputrerUI::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        drawPointSingleton::drawAction act = m_clipper->drawAction(e->pos());
        if(m_ptoolbar->currentSetting().shape != drawSetting::NONE){
            act = drawPointSingleton::PAINTING;
        }

        m_drawPoints->appendPoint(e->pos());
        m_drawPoints->setCurrentAction(act);

        if(act == drawPointSingleton::CREATE){
            clipStart();
        }
//        else if(act == drawPointSingleton::MOVE){
//            clipMoveStart();
//        }
    }
}

void LScreenCaputrerUI::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        m_drawPoints->appendPoint(e->pos());
        Qt::CursorShape curs = cursor().shape();
        switch (curs) {
        case Qt::CrossCursor:
        case Qt::SizeFDiagCursor:
        case Qt::SizeBDiagCursor:
            clipStart();
            break;

        case Qt::SizeAllCursor:
            clipMove();
            break;

        default:
            break;
        }
    }
    else{
        drawPointSingleton::drawAction act = m_clipper->drawAction(e->pos());
        if(m_ptoolbar->currentSetting().shape != drawSetting::NONE){
            act = drawPointSingleton::PAINTING;
        }
        switch (act) {
        case drawPointSingleton::CREATE:
            setCursor(Qt::CrossCursor);
            break;

        case drawPointSingleton::MOVE:
            setCursor(Qt::SizeAllCursor);
            break;

        case drawPointSingleton::PAINTING:
            setCursor(Qt::PointingHandCursor);
            break;

        case drawPointSingleton::TOP_LEFT_EXPAND:
        case drawPointSingleton::BOTTOM_RIGHT_EXPAND:
            setCursor(Qt::SizeFDiagCursor);
            break;

        case drawPointSingleton::TOP_RIGHT_EXPAND:
        case drawPointSingleton::BOTTOM_LEFT_EXPAND:
            setCursor(Qt::SizeBDiagCursor);
            break;

        default:
            unsetCursor();
            break;
        }


    }

    repaint();
}

void LScreenCaputrerUI::mouseReleaseEvent(QMouseEvent *e)
{
    m_drawPoints->appendPoint(e->pos());
    m_drawPoints->appendHistory();
//    if(m_drawPoints->current().action != drawPointSingleton::PAINTING)
    m_drawPoints->resetPath();
    clipFinish();
    repaint();
}

void LScreenCaputrerUI::onDrawSettingChanged(drawSetting setting)
{
    m_drawPoints->setCurrentSetting(setting);
    if(setting.shape != drawSetting::NONE){
        bool isVaild = false;
        QImage img = m_clipper->clipImage(isVaild);
        if(isVaild){
            m_painter->startPaint(m_clipper->clipTopLeft(), img);
        }
//        m_painter->resize(m_clipper->rect().size());
//        m_painter->move(m_clipper->clipTopLeft());
        m_painter->show();
        m_clipper->hide();
//        m_painter->onDrawSettingChanged(setting);
    }
    else{
        m_painter->hide();
    }
}


void LScreenCaputrerUI::clipStart()
{
    m_ptoolbar->hide();
}

void LScreenCaputrerUI::clipFinish()
{
    QPoint bottomleft = m_clipper->clipBottomLeft();
    bottomleft.setY(bottomleft.y() + m_ptoolbar->height());
//    m_painter->startPaint(m_clipper->clipTopLeft(), m_clipper->clipSize());

    m_ptoolbar->show();
    m_ptoolbar->move(bottomleft);
}

void LScreenCaputrerUI::clipMove()
{
    QPoint bottomleft = m_clipper->clipBottomLeft();
    bottomleft.setY(bottomleft.y() + m_ptoolbar->height());
    m_ptoolbar->show();
    m_ptoolbar->move(bottomleft);
}

void LScreenCaputrerUI::screenInit()
{
    QScreen* screen = QGuiApplication::primaryScreen();

    resize(screen->size());
    setMouseTracking(true);
}




