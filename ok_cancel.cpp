#include "ok_cancel.h"
#include "ui_ok_cancel.h"

ok_cancel::ok_cancel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ok_cancel)
{
    ui->setupUi(this);
    ui->ok->connect(ui->cancel, SIGNAL(clicked()), this, SLOT(close()));
}

ok_cancel::~ok_cancel()
{
    delete ui;
}

void ok_cancel::setSettings(const QString string, const QString st, const int type){
    this->st = st;
    this->type = type;
    ui->label->setText(string+st+"\"?");
}

void ok_cancel::on_ok_clicked()
{
    emit del(st, type);
    this->close();
}
