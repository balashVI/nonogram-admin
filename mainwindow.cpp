#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_settings();
    connect_to_db();
    add_context_menus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_all(){
    update_languages();
}

//Добавляє контекстні меню
void MainWindow::add_context_menus(){
    //Для таблиці мов
    QAction *act;
    act = new QAction("Перейменувати", ui->languages);
    ui->languages->addAction(act);
    ui->languages->connect(act, SIGNAL(triggered()), this, SLOT(lang_delete()));
    act = new QAction("Видалити", ui->languages);
    ui->languages->addAction(act);
    ui->languages->connect(act, SIGNAL(triggered()), this, SLOT(lang_delete()));
    ui->languages->setContextMenuPolicy(Qt::ActionsContextMenu);
}
