#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QAction>
#include <QMessageBox>
#include "ok_cancel.h"
#include "insert_text.h"

const int DEL_LANGUAGE = 1;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();
    void context_rename_language();
    void context_delete_language();
    void context_add_language();
    void context_selected_lang_changed();
    void activate_window(int result);
    void del(QString st, int type);
    void rename(QString newname, QString st, int type);

private:
    Ui::MainWindow *ui;
    void update_all();
    void read_settings();
    void connect_to_db();
    void update_languages();
    void add_context_menus();
    QSqlDatabase db;
    QString host, user_name;
    bool connected;

    int DEL_WORD = 2;
};

#endif // MAINWINDOW_H
