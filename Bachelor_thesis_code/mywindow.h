#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
extern "C"{
#include "ser.h"
#include "funct.h"
}
namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT
    static const int PCD=0;
    static const int PICC=1;
    static const int Default=0;
    static const int User=1;
    static const int BL=2;
public:
    explicit MyWindow(QWidget *parent = 0,QString un="you");
    ~MyWindow();
    void rebuildHistory();
    int getmode() {return C_mode;}
    int getrRegP() {return C_RegPara;}
    //void setReg();
private slots:
    void setReg();
    void on_B_portopen_clicked();

    void on_B_PCD_clicked();

    void on_B_PICC_clicked();

    void on_B_deafault_clicked();

    void on_B_user_clicked();
signals:
    void emitSetReg();
private:
    Ui::MyWindow *ui;
    QString ttyport; // : /dev/tty***
    int C_mode; //0:PCD 1:PICC
    int C_RegPara; //0:default 1:user
    QStringList historylog;
    QString username;
};

#endif // MYWINDOW_H
