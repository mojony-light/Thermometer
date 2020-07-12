#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->commitButton, &QPushButton::clicked, [ = ](){
        do {
            if (ui->lineEdit->text().isEmpty())
                break;

            ui->centerWidget->setThermometer(ui->lineEdit->text().toDouble());
        } while (0);
    });
}

Widget::~Widget()
{
    delete ui;
}

