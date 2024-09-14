#include "widget.hpp"
#include "./ui_widget.h"
#include <QDebug>
#include <QIcon>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget)
{
    this->ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));

    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::toogleShow);

    void (QSpinBox::*singalcal_changed)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBox, singalcal_changed, ui->horizontalSlider, &QSlider::setValue);
    // connect(ui->spinBox, &QSpinBox::valueChanged, ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
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
