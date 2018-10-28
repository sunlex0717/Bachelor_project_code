#include "dialoglogin.h"
#include "ui_dialoglogin.h"

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    setModal(true);
    item.Username=QString("sunlex");
    item.Password=QString("sw");
    ui->Password->setEchoMode(QLineEdit::Password);
    errordlg =new QErrorMessage(this);
    /*
    errordlg->setWindowTitle("Error!");
    errordlg->showMessage("Please enter correct passoword/username!");*/
}

DialogLogin::~DialogLogin()
{
    delete errordlg;
    delete ui;
}

void DialogLogin::on_Login_clicked()
{
    if((item.Username==ui->Username->text())&&
            (item.Password==ui->Password->text()))
        accept();
    else{
        errordlg->setWindowTitle("Error!");
        errordlg->showMessage("Please enter correct passoword/username!");
    }
}

void DialogLogin::on_Cancel_clicked()
{
    reject();
}

void DialogLogin::setUserInfo(UserInfo us)
{
    item.Username=us.Username;
    item.Password=us.Password;
}

QString DialogLogin::getUserName()const
{
    return item.Username;
}
