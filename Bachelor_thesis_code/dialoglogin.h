#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include <QString>
#include <QErrorMessage>
class QErrorMessage;
namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT
    typedef struct userinfo{
        QString Username;
        QString Password;
    }   UserInfo;

public:
    explicit DialogLogin(QWidget *parent = 0);
    ~DialogLogin();
    QString getUserName()const;
    void setUserInfo(UserInfo us);

private slots:
    void on_Login_clicked();

    void on_Cancel_clicked();

private:
    Ui::DialogLogin *ui;

    //UserInfo *Items;
    //int n_item;
    UserInfo item;
    QErrorMessage *errordlg;
};

#endif // DIALOGLOGIN_H
