#include "ui_mainwindow.h"
#include "mainwindow.h"

//Добавляє контекстні меню
void MainWindow::add_context_menus(){
    //Для таблиці мов
    QAction *act;
    act = new QAction("Добавити", ui->languages);
    ui->languages->addAction(act);
    ui->languages->connect(act, SIGNAL(triggered()), this, SLOT(context_add_language()));
    act = new QAction("Перейменувати", ui->languages);
    ui->languages->addAction(act);
    ui->languages->connect(act, SIGNAL(triggered()), this, SLOT(context_rename_language()));
    act = new QAction("Видалити", ui->languages);
    ui->languages->addAction(act);
    ui->languages->connect(act, SIGNAL(triggered()), this, SLOT(context_delete_language()));
    ui->languages->actions().at(1)->setEnabled(false);
    ui->languages->actions().at(2)->setEnabled(false);
    ui->languages->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->languages->setSelectionMode(QAbstractItemView::SingleSelection);
}

//Оновлює дані в розділі "Локалізації"
void MainWindow::update_languages(){
    QSqlQuery query;
    QSqlQueryModel *quer = new QSqlQueryModel(this);
    quer->setQuery("select name from languages order by name", db);
    quer->setHeaderData(0, Qt::Horizontal, QObject::trUtf8("Мова"));
    ui->languages->setModel(quer);
    connect(ui->languages->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(context_selected_lang_changed()));
    context_selected_lang_changed();
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

void MainWindow::context_delete_language(){
    ok_cancel *window = new ok_cancel(this);
    window->setSettings(trUtf8("Видалити мову \""),
          ui->languages->selectionModel()->selectedRows().at(0).data().toString(),DEL_LANGUAGE);
    connect(window, SIGNAL(del(QString,int)), this, SLOT(del(QString,int)));
    window->show();
    ui->centralWidget->setEnabled(false);
    window->connect(window, SIGNAL(finished(int)), this, SLOT(activate_window(int)));
}

void MainWindow::context_add_language(){
    ui->new_language->setFocus();
}

void MainWindow::context_rename_language(){
    insert_text *rename = new insert_text(this);
    ui->centralWidget->setEnabled(false);
    rename->setSettings(trUtf8("Нова назва для мови \""),
                        ui->languages->selectionModel()->selectedRows().at(0).data().toString(),DEL_LANGUAGE);
    connect(rename, SIGNAL(rename(QString,QString,int)), this, SLOT(rename(QString,QString,int)));
    rename->show();
    connect(rename, SIGNAL(finished(int)), this, SLOT(activate_window(int)));
}

void MainWindow::activate_window(int result){
    ui->centralWidget->setEnabled(true);
}

//Видаляє записи з таблиць
void MainWindow::del(QString st, int type){
    switch (type){
        case DEL_LANGUAGE:
            db.exec("delete from languages where name='"+st+"'");
            update_languages();
        break;
    }
}

//Змінює записи в таблицях
void MainWindow::rename(QString newname, QString st, int type){
    switch (type){
        case DEL_LANGUAGE:
            QString str = newname;
            str.remove(" ");
            if(str==""){
                QMessageBox::critical(NULL, "Помилка!", "Введена некоректна назва мови!");
                return;
            }
            db.exec("update languages set name='"+newname+"' where name='"+st+"'");
            update_languages();
        break;
    }
}

void MainWindow::context_selected_lang_changed(){
   bool enable = ui->languages->selectionModel()->selectedRows().size();
   ui->languages->actions().at(1)->setEnabled(enable);
   ui->languages->actions().at(2)->setEnabled(enable);
}
