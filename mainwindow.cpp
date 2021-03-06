#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    add_context_menus();
    read_settings();
    connect_to_db();
    ui->languages->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_words->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_translated_words->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_translated_words->setSelectionBehavior( QAbstractItemView::SelectRows );
    new_crossword = new crossword();
    ui->new_crossword_scroll->setWidget(new_crossword);
}

MainWindow::~MainWindow()
{
    delete new_crossword;
    delete ui;
    db.close();
}

void MainWindow::update_all(){
    update_languages();
}

//Зчитує налаштування з локальної БД
void MainWindow::read_settings(){
    db = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    db.setDatabaseName("settings.sqlite");
    bool open = db.open();
    if(open){
        db.exec("CREATE TABLE IF NOT EXISTS `settings` ("
                "`property` text PRIMARY KEY,"
                "`value` text NOT NULL"
                ")");
        QSqlQuery query = db.exec("select * from settings");
        if(query.next()){
            host = query.value(1).toString();
            query.next();
            user_name = query.value(1).toString();
        }else{
            db.exec("insert into settings(property, value) values('host', 'localhost')");
            db.exec("insert into settings(property, value) values('user', 'non_admin')");
            host = "localhost";
            user_name = "non_admin";
        }
        db.close();
    }else{
        QMessageBox::critical(NULL, "Помилка читання налаштувань!", db.lastError().text());
        exit(0);
    }
}

//Встановлює з'єднання з віддаленою БД
void MainWindow::connect_to_db(){
    db = QSqlDatabase::addDatabase("QMYSQL", "db");
    db.setHostName(host);
    db.setDatabaseName("nonogram");
    db.setUserName(user_name);
    db.setPassword("admin");
    connected = db.open();
    if(connected){
        update_all();

    }else{
        QMessageBox::critical(NULL, "Не вдалося підключитися до бази даних!", db.lastError().text());
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    new_crossword->setSize(ui->spinBox_width->value(), ui->spinBox_height->value());
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    new_crossword->set_cell_size(value);
    new_crossword->draw_layout();
    new_crossword->repaint();
}
