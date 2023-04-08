#include "widget.h"
#include <windows.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
