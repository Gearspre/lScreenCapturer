#include "lscreencaputrer_ui.h"
#include "ui_lscreencaputrer_ui.h"

#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QWindow>
#include <QImage>
#include <QDebug>

LScreenCaputrerUI::LScreenCaputrerUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LScreenCaputrerUI)
{
    ui->setupUi(this);
    resize(600, 400);

    screenInit();
//    test = new BasePainter(this);
}

LScreenCaputrerUI::~LScreenCaputrerUI()
{
    delete ui;
}

void LScreenCaputrerUI::mousePressEvent(QMouseEvent *e)
{
    m_path.append(e->pos());
}

void LScreenCaputrerUI::mouseMoveEvent(QMouseEvent *e)
{
    m_path.append(e->pos());
    repaint();
}

void LScreenCaputrerUI::mouseReleaseEvent(QMouseEvent *e)
{
    m_path.append(e->pos());
    repaint();
}

void LScreenCaputrerUI::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(0, 0, 0, 50)));
    painter.setClipping(true);

    painter.drawImage(0,0,m_pmap.toImage());
    painter.drawRect(rect());
    if(m_path.count() >= 2){
        const qint32 lastX = m_path.last().x();
        const qint32 lastY =m_path.last().y();
        const qint32 firstX = m_path.first().x();
        const qint32 firstY = m_path.first().y();

        const qint32 width = lastX - firstX;
        const qint32 height = lastY - firstY;
        const qint32 drawX = width > 0 ? firstX : lastX;
        const qint32 limitX = width > 0 ? lastX : firstX;
        const qint32 drawY = height > 0 ? firstY : lastY;
        const qint32 limitY = height > 0 ? lastY : firstY;


        QImage clipImg(QSize(qAbs(width), qAbs(height)), QImage::Format_ARGB32);

//        qDebug()<< "====";
//        qDebug()<< "width "<<width;
//        qDebug()<< "height "<<height;
//        qDebug()<< "limitHeight "<<limitHeight;
//        qDebug()<< "limitWidth "<<limitWidth;
//        qDebug()<< "----";

        //row by column - left to Right
        if(width > 0 && height > 0){
            for(qint32 clipY = drawY, srcY = 0; clipY < limitY; clipY++, srcY++){
                qint32 srcX = 0;
                for(qint32 clipX = firstX; clipX < lastX; clipX++,srcX++){
                    clipImg.setPixel(srcX, srcY, m_srcMap.pixel(clipX, clipY));
                }
            }

        }

        else if(width < 0 && height > 0){
            for(qint32 clipY = firstY, srcY = 0; clipY < lastY; clipY++, srcY++){
                qint32 srcX = 0;
                for(qint32 clipX = lastX; clipX < firstX; clipX++, srcX++){
                    clipImg.setPixel(srcX, srcY, m_srcMap.pixel(clipX, clipY));
                }
            }
//            painter.drawImage(QRect(lastX,firstY, width, height), clipImg);
        }



        else if(width > 0 && height < 0){
            for(qint32 clipY = lastY, srcY = 0; clipY < firstY; clipY++, srcY++){
                qint32 srcX = 0;
                for(qint32 clipX = firstX; clipX < lastX; clipX++, srcX++){
                    clipImg.setPixel(srcX, srcY, m_srcMap.pixel(clipX, clipY));
                }
            }
//                painter.drawImage(QRect(firstX,lastY, width, height), clipImg);
        }

        else if(width < 0 && height < 0){
            for(qint32 clipY = lastY, srcY = 0; clipY < firstY; clipY++, srcY++){
                qint32 srcX = 0;
                for(qint32 clipX = lastX; clipX < firstX; clipX++, srcX++){
                    clipImg.setPixel(srcX, srcY, m_srcMap.pixel(clipX, clipY));
                }
            }
//            painter.drawImage(QRect(lastX,lastY, width, height), clipImg);
        }

        if(width && height){
            painter.drawImage(QRect(drawX,drawY, width, height), clipImg);
        }
    }

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



