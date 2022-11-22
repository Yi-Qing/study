#include "widget.hpp"
#include "./ui_widget.h"
#include <QDebug>
#include <QIcon>
#include <QWidget>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget)
{
    this->ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));

    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::toogleShow);
}

void Widget::toogleShow()
{
    static bool flag = false;

    if (!flag) {
        ui->lineEdit->setText("hello world");
        qDebug() << "need show";
        flag = true;
    } else {
        ui->lineEdit->clear();
        qDebug() << "need hide";
        flag = false;
    }
}

Widget::~Widget()
{
    delete ui;
}
