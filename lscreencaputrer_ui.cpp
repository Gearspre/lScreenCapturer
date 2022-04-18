#include "lscreencaputrer_ui.h"
#include "ui_lscreencaputrer_ui.h"

#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QWindow>
#include <QImage>
#include <QDebug>

#include "drawpoints.h"

LScreenCaputrerUI::LScreenCaputrerUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LScreenCaputrerUI)
{
    m_drawPoints = m_drawPoints->getInstance();
    ui->setupUi(this);
    resize(600, 400);

//    screenInit();
    setAttribute( Qt::WA_Hover,true);
    test1 = new LScreenClipper(this);
    setMouseTracking(true);
    test1->setAttribute(Qt::WA_TransparentForMouseEvents, true);
//    test = new BasePainter(this);
}

LScreenCaputrerUI::~LScreenCaputrerUI()
{
    delete ui;
}

void LScreenCaputrerUI::mousePressEvent(QMouseEvent *e)
{
    m_drawPoints->appendPoint(e->pos());
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
//    m_drawPoints->appendHistory();
    repaint();
}

void LScreenCaputrerUI::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)



}

void LScreenCaputrerUI::screenInit()
{
    m_currentScreen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        m_currentScreen = window->screen();
    m_pmap = m_currentScreen->grabWindow(0);
    m_srcMap = m_pmap.copy().toImage();
//    if (!m_currentScreen)
//        return;
}



