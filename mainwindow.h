#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QAction>
#include <QMessageBox>
#include "ok_cancel.h"
#include "insert_text.h"
#include "crossword.h"

const int LANGUAGE = 1;
const int WORD = 2;
const int TRANSLATION = 3;

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
    void context_selected_word_changed();
    void context_selected_translation_changed();
    void context_add_word();
    void context_delete_word();
    void context_rename_word();
    void context_delete_translation();
    void context_change_translation();
    void activate_window(int result);
    void del(QString st, int type);
    void rename(QString newname, QString st, int type);
    void on_pushButton_4_clicked();

    void on_comboBox_lang_currentIndexChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

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
    crossword *new_crossword;
};

#endif // MAINWINDOW_H
