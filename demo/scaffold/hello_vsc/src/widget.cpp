#include "widget.hpp"
#include "./ui_widget.h"
#include <qwidget.h>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::toogleShow);
}

void Widget::toogleShow()
{
    static bool flag = false;

    if (!flag) {
        ui->lineEdit->setText("hello world");
        flag = true;
    } else {
        ui->lineEdit->clear();
        flag = false;
    }
}

Widget::~Widget()
{
    delete ui;
}
