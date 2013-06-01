#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_settings();
    connect_to_db();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_all(){
    update_languages();
}
