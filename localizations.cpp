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
    //Для таблиці слів
    act = new QAction("Добавити", ui->languages);
    ui->tableView_words->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(context_add_word()));
    act = new QAction("Перейменувати", ui->languages);
    ui->tableView_words->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(context_rename_word()));
    act = new QAction("Видалити", ui->languages);
    ui->tableView_words->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(context_delete_word()));
    ui->tableView_words->actions().at(1)->setEnabled(false);
    ui->tableView_words->actions().at(2)->setEnabled(false);
    ui->tableView_words->setContextMenuPolicy(Qt::ActionsContextMenu);
    //Для таблиці перекладів
    act = new QAction("Змінити", ui->languages);
    ui->tableView_translated_words->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(context_change_translation()));
    act = new QAction("Видалити", ui->languages);
    ui->tableView_translated_words->addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(context_delete_translation()));
    ui->tableView_translated_words->actions().at(0)->setEnabled(false);
    ui->tableView_translated_words->actions().at(1)->setEnabled(false);
    ui->tableView_translated_words->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//Оновлює дані в розділі "Локалізації"
void MainWindow::update_languages(){
    //Мови
    QSqlQueryModel *quer = new QSqlQueryModel(this);
    quer->setQuery("select name from languages order by name", db);
    quer->setHeaderData(0, Qt::Horizontal, QObject::trUtf8("Мова"));
    ui->languages->setModel(quer);
    connect(ui->languages->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(context_selected_lang_changed()));
    context_selected_lang_changed();

    //Слова
    quer = new QSqlQueryModel(this);
    quer->setQuery("select word from words order by word", db);
    quer->setHeaderData(0, Qt::Horizontal, QObject::trUtf8("Слово"));
    ui->tableView_words->setModel(quer);
    connect(ui->tableView_words->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(context_selected_word_changed()));
    context_selected_word_changed();

    //Переклади
    QSqlQuery query= db.exec("select name from languages order by name");
    ui->comboBox_lang->clear();
    while(query.next())
        ui->comboBox_lang->addItem(query.value(0).toString());
    context_selected_translation_changed();
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
          ui->languages->selectionModel()->selectedRows().at(0).data().toString(),LANGUAGE);
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
                        ui->languages->selectionModel()->selectedRows().at(0).data().toString(),LANGUAGE);
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
        case LANGUAGE:
            db.exec("delete from languages where name='"+st+"'");
            update_languages();
        break;
        case WORD:
            db.exec("delete from words where word='"+st+"'");
            update_languages();
        break;
        case TRANSLATION:
            QSqlQuery query = db.exec("select id from languages where name='"+ui->comboBox_lang->currentText()+"'");
            query.next();
            int lang_id=query.value(0).toInt();
            query = db.exec("select id from words where word='"+st+"'");
            query.next();
            int word_id=query.value(0).toInt();
            db.exec("delete from vocabulary where id_language='"+QString::number(lang_id)+"' and id_word='"+
                    QString::number(word_id)+"'");
            int i = ui->comboBox_lang->currentIndex();
            update_languages();
            ui->comboBox_lang->setCurrentIndex(i);
        break;
    }
}

//Змінює записи в таблицях
void MainWindow::rename(QString newname, QString st, int type){
    QString str;
    switch (type){
        case LANGUAGE:
            str = newname;
            str.remove(" ");
            if(str==""){
                QMessageBox::critical(NULL, "Помилка!", "Введена некоректна назва мови!");
                return;
            }
            db.exec("update languages set name='"+newname+"' where name='"+st+"'");
            update_languages();
        break;
        case WORD:
            str = newname;
            str.remove(" ");
            if(str==""){
                QMessageBox::critical(NULL, "Помилка!", "Введена некоректна назва слова!");
                return;
            }
            db.exec("update words set word='"+newname+"' where word='"+st+"'");
            update_languages();
        break;
        case TRANSLATION:
            str = newname;
            str.remove(" ");
            if(str==""){
                QMessageBox::critical(NULL, "Помилка!", "Введий некоректний переклад!");
                return;
            }
            QSqlQuery query = db.exec("select id from languages where name='"+ui->comboBox_lang->currentText()+"'");
            query.next();
            int lang_id=query.value(0).toInt();
            query = db.exec("select id from words where word='"+st+"'");
            query.next();
            int word_id=query.value(0).toInt();
            db.exec("update vocabulary set translation='"+newname+"' where id_language='"+QString::number(lang_id)+
                    "' and id_word='"+QString::number(word_id)+"'");
            int i = ui->comboBox_lang->currentIndex();
            update_languages();
            ui->comboBox_lang->setCurrentIndex(i);
        break;
    }
}

void MainWindow::context_selected_lang_changed(){
   bool enable = ui->languages->selectionModel()->selectedRows().size();
   ui->languages->actions().at(1)->setEnabled(enable);
   ui->languages->actions().at(2)->setEnabled(enable);
}

//.......................................Слова.......................................
void MainWindow::on_pushButton_4_clicked()
{
    QString str = ui->new_word->text();
    str.remove(" ");
    if(str==""){
        QMessageBox::critical(NULL, "Помилка!", "Введена некоректна назва нового слова!");
        ui->new_word->setText("");
        return;
    }
    db.exec("insert into words set word='"+ui->new_word->text()+"'");
    ui->new_word->setText("");
    update_languages();
}

void MainWindow::context_selected_word_changed(){
    bool enable = ui->tableView_words->selectionModel()->selectedRows().size();
    ui->tableView_words->actions().at(1)->setEnabled(enable);
    ui->tableView_words->actions().at(2)->setEnabled(enable);
}

void MainWindow::context_add_word(){
    ui->new_word->setFocus();
}

void MainWindow::context_delete_word(){
    ok_cancel *window = new ok_cancel(this);
    window->setSettings(trUtf8("Видалити слово \""),
          ui->tableView_words->selectionModel()->selectedRows().at(0).data().toString(),WORD);
    connect(window, SIGNAL(del(QString,int)), this, SLOT(del(QString,int)));
    window->show();
    ui->centralWidget->setEnabled(false);
    window->connect(window, SIGNAL(finished(int)), this, SLOT(activate_window(int)));
}

void MainWindow::context_rename_word(){
    insert_text *rename = new insert_text(this);
    ui->centralWidget->setEnabled(false);
    rename->setSettings(trUtf8("Нова назва для слова \""),
                        ui->tableView_words->selectionModel()->selectedRows().at(0).data().toString(),WORD);
    connect(rename, SIGNAL(rename(QString,QString,int)), this, SLOT(rename(QString,QString,int)));
    rename->show();
    connect(rename, SIGNAL(finished(int)), this, SLOT(activate_window(int)));
}

//.......................................Переклади...................................
void MainWindow::on_comboBox_lang_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query = db.exec("select id from languages where name='"+arg1+"'");
    if(!query.next())return;
    int lang_id=query.value(0).toInt();
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(tr("select words.word, vocabulary.translation from vocabulary right join words ")+
                       tr("on words.id=vocabulary.id_word and vocabulary.id_language='")+QString::number(lang_id)+tr("'"), db);
    model->setHeaderData(0, Qt::Horizontal, QObject::trUtf8("Слово"));
    model->setHeaderData(1, Qt::Horizontal, QObject::trUtf8("Переклад"));
    ui->tableView_translated_words->setModel(model);
    query = db.exec("select words.word from vocabulary right join words on vocabulary.id_word=words.id and vocabulary.id_language="+QString::number(lang_id)+" where vocabulary.translation is null");
    ui->comboBox_not_translated_word->clear();
    while(query.next())
        ui->comboBox_not_translated_word->addItem(query.value(0).toString());

    if(ui->comboBox_not_translated_word->currentText()==""){
        ui->pushButton_5->setEnabled(false);
        ui->new_translation->setEnabled(false);
    } else{
        ui->new_translation->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
    }
    connect(ui->tableView_translated_words->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(context_selected_translation_changed()));
}

void MainWindow::on_pushButton_5_clicked()
{
    QString str = ui->new_translation->text();
    str.remove(" ");
    if(str==""){
        QMessageBox::critical(NULL, "Помилка!", "Введений некоректний переклад!");
        ui->new_translation->setText("");
        return;
    }
    QSqlQuery query = db.exec("select id from languages where name='"+ui->comboBox_lang->currentText()+"'");
    query.next();
    int lang_id=query.value(0).toInt();
    query = db.exec("select id from words where word='"+ui->comboBox_not_translated_word->currentText()+"'");
    query.next();
    int word_id=query.value(0).toInt();
    db.exec("insert into vocabulary(id_language, id_word, translation) values("+
            QString::number(lang_id)+", "+QString::number(word_id)+", '"+ui->new_translation->text()+"')");
    ui->new_translation->setText("");
    int i = ui->comboBox_lang->currentIndex();
    update_languages();
    ui->comboBox_lang->setCurrentIndex(i);
}

void MainWindow::context_delete_translation(){
    ok_cancel *window = new ok_cancel(this);
    window->setSettings(trUtf8("Видалити переклад слова \""),
          ui->tableView_translated_words->selectionModel()->selectedRows().at(0).data().toString(),TRANSLATION);
    connect(window, SIGNAL(del(QString,int)), this, SLOT(del(QString,int)));
    window->show();
    ui->centralWidget->setEnabled(false);
    window->connect(window, SIGNAL(finished(int)), this, SLOT(activate_window(int)));
}

void MainWindow::context_selected_translation_changed(){
    int size = ui->tableView_translated_words->selectionModel()->selectedRows().size();
    if(size){
        if(!ui->tableView_translated_words->selectionModel()->selectedRows(1).first().data().toString().isEmpty()){
            ui->tableView_translated_words->actions().at(0)->setEnabled(true);
            ui->tableView_translated_words->actions().at(1)->setEnabled(true);
        }else{
            ui->tableView_translated_words->actions().at(0)->setEnabled(false);
            ui->tableView_translated_words->actions().at(1)->setEnabled(false);
        }
    }else{
        ui->tableView_translated_words->actions().at(0)->setEnabled(false);
        ui->tableView_translated_words->actions().at(1)->setEnabled(false);
    }
}

void MainWindow::context_change_translation(){
    insert_text *rename = new insert_text(this);
    ui->centralWidget->setEnabled(false);
    rename->setSettings(trUtf8("Новий переклад для слова \""),
                        ui->tableView_translated_words->selectionModel()->selectedRows().at(0).data().toString(),TRANSLATION);
    connect(rename, SIGNAL(rename(QString,QString,int)), this, SLOT(rename(QString,QString,int)));
    rename->show();
    connect(rename, SIGNAL(finished(int)), this, SLOT(activate_window(int)));
}
