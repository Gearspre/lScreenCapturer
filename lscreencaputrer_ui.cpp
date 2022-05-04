#include "lscreencaputrer_ui.h"
#include "ui_lscreencaputrer_ui.h"

#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
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
    , m_systemTray(new QSystemTrayIcon(this))
{
    ui->setupUi(this);
    screenInit();
    systemTrayInit();
    toolbarInit();
    keyCaptureInit();

    m_painter->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_clipper->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->grabKeyboard();
//    this->hide();
    this->resize(0, 0);
    m_clipper->hide();
}

LScreenCaputrerUI::~LScreenCaputrerUI()
{
    delete ui;
    if(m_ptoolbar)
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
    m_drawPoints->resetPath();
    clipFinish();
    repaint();
}

void LScreenCaputrerUI::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    if(!m_isSleep){
        QPainter painter(this);
        painter.setBrush(QBrush(QColor(0, 0, 0, 50)));

        painter.drawImage(0,0,m_background);
        painter.drawRect(rect());
    }
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
        else{
            QMessageBox::critical(this, "error", "causing unknown error when get screnn image");
            return;
        }
        m_painter->show();
        m_clipper->hide();
    }
    else{
        m_painter->hide();
        m_clipper->show();
    }
}

void LScreenCaputrerUI::onKeyBoardPressed(int key)
{
    m_keyHash.insert(key, true);

    if(m_keyHash.value(27) && !m_isSleep){
        clipCancel();
    }
    else if(m_keyHash.count() >= 3){
        bool check = false;
        check = m_keyHash.value(VK_LCONTROL);
        check = check && m_keyHash.value(VK_LSHIFT);
        check = check && m_keyHash.value(Qt::Key_A);

        if(check){
            m_keyHash.clear();
            clipStart();
        }
    }
}

void LScreenCaputrerUI::onKeyBoardRelease(int key)
{
    m_keyHash.remove(key);
}

void LScreenCaputrerUI::onSigSave()
{
    const QString defPath = QString("/ScreenCapture_%1.%2").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")).arg("png");
    QString sPath = QFileDialog::getSaveFileName(this, "Save Image",
                                                defPath,
                                                "Images (*.png *.jpg)");
    qDebug() << sPath;
}


void LScreenCaputrerUI::clipStart()
{
    this->resize(m_background.size());
    this->move(0, 0);
    m_isSleep = false;
    m_clipper->show();
    m_ptoolbar->hide();
}

void LScreenCaputrerUI::clipFinish()
{
    QPoint bottomleft = m_clipper->clipBottomLeft();
//    m_painter->startPaint(m_clipper->clipTopLeft(), m_clipper->clipSize());

    m_ptoolbar->show();
    m_ptoolbar->move(bottomleft);
}

void LScreenCaputrerUI::clipMove()
{
    QPoint bottomleft = m_clipper->clipBottomLeft();
//    bottomleft.setY(bottomleft.y() - m_ptoolbar->height());
    m_ptoolbar->show();
    m_ptoolbar->move(bottomleft);
}

void LScreenCaputrerUI::clipCancel()
{
    qDebug() << "clipCancel";
//    this->resize(0, 0);
    m_isSleep = true;
    m_ptoolbar->hide();
    m_clipper->hide();
    m_drawPoints->resetAll();
    m_clipper->reset();
    this->setFocus();
}

void LScreenCaputrerUI::screenInit()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    m_background = screen->grabWindow(0).toImage();

    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(screen->size());
    setMouseTracking(true);
}

void LScreenCaputrerUI::systemTrayInit()
{
    m_systemTray->setIcon(QIcon(":/icon/icon/clipper.svg"));
    m_systemTray->setToolTip("LScreenCapturer截图助手");
    m_systemTray->show();
    m_systemTray->showMessage("提示", "软件启动成功");

    QMenu* menu = new QMenu(this);
    QAction* quit = new QAction(menu);
    quit->setText("关闭软件");
    connect(quit, &QAction::triggered, [](){
        exit(0);
    });

    QAction* help = new QAction(menu);
    help->setText("帮助");
    connect(help, &QAction::triggered, [this](){
        QMessageBox::information(this, "快捷键", "Ctrl + Shift + A 开启截图模式\nCtrl + C 复制截图至剪贴板\nEsc 退出截图模式");
    });

    menu->addAction(help);
    menu->addAction(quit);
    m_systemTray->setContextMenu(menu);
}

void LScreenCaputrerUI::toolbarInit()
{
    connect(m_ptoolbar, SIGNAL(settingChanged(drawSetting)), this, SLOT(onDrawSettingChanged(drawSetting)));
    connect(m_ptoolbar, &LScreenPainterToolBar::sigClose, [this](){
        clipCancel();
    } );
    connect(m_ptoolbar, SIGNAL(sigSave()), this, SLOT(onSigSave()));
}

void LScreenCaputrerUI::keyCaptureInit()
{
    m_keyCapturer = LKeyCapturer::getInstance();
    connect(m_keyCapturer, SIGNAL(KeyPressed(int)), this, SLOT(onKeyBoardPressed(int)));
}




