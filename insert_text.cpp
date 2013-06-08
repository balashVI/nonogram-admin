#include "insert_text.h"
#include "ui_insert_text.h"

insert_text::insert_text(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insert_text)
{
    ui->setupUi(this);
    ui->ok->connect(ui->cancel, SIGNAL(clicked()), this, SLOT(close()));
}

insert_text::~insert_text()
{
    delete ui;
}

void insert_text::setSettings(QString label, QString st, int type){
    ui->label->setText(label+st+"\":");
    this->st=st;
    this->type=type;
}

void insert_text::on_ok_clicked()
{
    emit rename(ui->lineEdit->text(), st, type);
    this->close();
}
