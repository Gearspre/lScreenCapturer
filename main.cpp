#include "lscreencaputrer_ui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LScreenCaputrerUI w;
    w.show();
    return a.exec();
}
