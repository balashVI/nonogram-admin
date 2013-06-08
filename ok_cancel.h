#ifndef OK_CANCEL_H
#define OK_CANCEL_H

#include <QDialog>
#include <QString>

namespace Ui {
class ok_cancel;
}

class ok_cancel : public QDialog
{
    Q_OBJECT
    
public:
    explicit ok_cancel(QWidget *parent = 0);
    ~ok_cancel();
    void setSettings(const QString string, const QString st, const int type);

signals:
    void del(QString, int);
    
private slots:
    void on_ok_clicked();

private:
    Ui::ok_cancel *ui;
    QString st;
    int type;
};

#endif // OK_CANCEL_H
