#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "QMessageBox"

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

//Оновлює дані в розділі "Локалізації"
void MainWindow::update_languages(){
    QSqlQuery query = db.exec("select count(id) from languages");
    query.next();
    ui->languages_count->setText(query.value(0).toString());
    query = db.exec("select count(id) from words");
    query.next();
    ui->words_count->setText(query.value(0).toString());
    QSqlQueryModel *quer = new QSqlQueryModel(this);
    quer->setQuery("select name from languages order by name", db);
    quer->setHeaderData(0, Qt::Horizontal, QObject::trUtf8("Мова"));
    //Чи потрібно видаляти попередню модель??????????????????????????????????????????????????????????????????????????????
    ui->languages->setModel(quer);
    ui->previous_language->clear();
    ui->deleted_language->clear();
    query = db.exec("select name from languages order by name");
    while(query.next()){
        ui->previous_language->addItem(query.value(0).toString());
        ui->deleted_language->addItem(query.value(0).toString());
    }
}

//Добавляє нову мову
void MainWindow::on_pushButton_clicked()
{
    QString str = ui->new_language->text();
    str.remove(" ");
    if(str==""){
        QMessageBox::critical(NULL, "Помилка!", "Введена некоректна назва нової мови!");
        ui->new_language->setText("");
        return;
    }
    db.exec("insert into languages set name='"+ui->new_language->text()+"'");
    ui->new_language->setText("");
    update_languages();
}

//Перейменовує мову
void MainWindow::on_pushButton_2_clicked()
{
    QString str = ui->new_language_name->text();
    str.remove(" ");
    if(str==""){
        QMessageBox::critical(NULL, "Помилка!", "Введена некоректна нова назва мови!");
        ui->new_language_name->setText("");
        return;
    }
    db.exec("update languages set name = '" + ui->new_language_name->text()+"' where name = '"
            +ui->previous_language->currentText()+"'");
    ui->new_language_name->setText("");
    update_languages();
}

//Видаляє наявну мову
void MainWindow::on_pushButton_3_clicked()
{
    if(ui->deleted_language->count()==0){
        QMessageBox::critical(NULL, "Помилка!", "Список мов пустий!");
        return;
    }
    db.exec("delete from languages where name = '"+ui->deleted_language->currentText()+"'");
    update_languages();
}
