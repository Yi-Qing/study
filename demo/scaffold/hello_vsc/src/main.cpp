#include "widget.hpp"

#include <QApplication>
#include <QDebug>
#include <stdio.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Widget w;
    qDebug("hello world");

    w.show();
    return a.exec();
}
