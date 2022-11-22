#include "widget.hpp"

#include <QApplication>
#include <QDebug>
#include <stdio.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();
    return a.exec();
}
