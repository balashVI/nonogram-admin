#ifndef INSERT_TEXT_H
#define INSERT_TEXT_H

#include <QDialog>

namespace Ui {
class insert_text;
}

class insert_text : public QDialog
{
    Q_OBJECT
    
public:
    explicit insert_text(QWidget *parent = 0);
    ~insert_text();
    void setSettings(QString label, QString st, int type);

signals:
    void rename(QString, QString, int);

private slots:
    void on_ok_clicked();

private:
    Ui::insert_text *ui;
    QString st;
    int type;
};

#endif // INSERT_TEXT_H
