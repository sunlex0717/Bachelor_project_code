#include "mywindow.h"
#include "ui_mywindow.h"
#include "dialogpcd.h"
#include "dialogpicc.h"
#include "dialoglogin.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string>
#include <QDebug>
extern "C"{
#include "ser.h"
#include "funct.h"
}
#include <iostream>
extern unchar sdata[SIZE];   //ra // complete
extern unchar cdata[SIZE];   //rb // raw
extern unchar payload[SIZE];  //rc//show
extern unchar rdata[SIZE];  //rd// recieve

extern unint ra,rb,rc,rd,i,j;
extern unint sum; //TFI+... ...+PDn
extern unchar state_response;
extern unint state_ack;
extern unint error_flag;
extern unchar Tg;

//static void setReg();

MyWindow::MyWindow(QWidget *parent,QString un) :
    QMainWindow(parent),
    ui(new Ui::MyWindow),
    C_mode(PCD),C_RegPara(Default)
{
    ui->setupUi(this);
    ui->TtyPort->setFocus();
    connect(this,SIGNAL(emitSetReg()),this,SLOT(setReg()));
    username=un;
    this->setWindowTitle("S system");
    QString wel="welcome!";
    historylog.append(wel+username);
    rebuildHistory();
}

MyWindow::~MyWindow()
{
    delete ui;
}
void MyWindow::rebuildHistory()
{
    QString history=historylog.join(QLatin1String("\n" ));
    ui->M_log->setText(history);
}

void MyWindow::on_B_portopen_clicked()//portopen and wake up PN532
{
    using std::string;
    //ui->TtyPort->setFocus();
    QString path=ui->TtyPort->text();
    //a method to convert QString to String and char *
    string s_path=path.toStdString();
    int len=s_path.length();
    char *c_path=new char[len+1];
    s_path.copy(c_path,len,0);
    c_path[len]='\0';
    //qDebug()<<c_path;
    if(ser_open(c_path))
    {

        qDebug()<<"ser_open() failed\n";
        QString s1="ser_open() failed";
        //ui->M_log->setText(s1);
        QString s2="you may need super user authority!";
        //ui->M_log->setText(s2);
        historylog.append(s1);
        historylog.append(s2);
        rebuildHistory();
    }
    else
    {
        qDebug()<<"port opened\n";
        QString s1="port opened";
        historylog.append(s1);

        int j;
        qDebug()<<"Start to wake up PN532... ...\n";
        QString msg1="Start to wake up PN532... ...";
        historylog.append(msg1);
        rebuildHistory();
        sleep(1);
        i=0;
        while(1)
        {
            j=wake_up();
            if(j==0){
                QString msg="Wake up succeeded!";
                historylog.append(msg);
                break;
            }
            if(i==5)
            {
                qDebug()<<"wake up failed\n";
                exit(0);
            }
            i++;
            sleep(0.1);
        }
        ra=0;
        cdata[ra++]=0xD4;
        cdata[ra++]=0x14; //SAMConfiguration
        cdata[ra++]=0x01; //Normal mode SAM not used
        send_command(ra);
        usleep(8000);
        rc=test_response();
        sleep(0.1);

    }
    rebuildHistory();

}

void MyWindow::on_B_PCD_clicked()//pcd
{
    C_mode=PCD;
    //DialogPCD *dlgPCD = new DialogPCD(this);
    error_flag=Authentication_KeyA(BL);
    //dlgPCD->setmode(this->getmode());
    //dlgPCD->setRegP(this->getrRegP());
    if(error_flag==1) {
        QString msg="the ke is wrong,Authentication failed! ";
        historylog.append(msg);
        rebuildHistory();
        //delete dlgPCD;
    }
    else{

        QString msg1="The key is right,Authentication succeeded!";
        historylog.append(msg1);
        rebuildHistory();
        error_flag=reading(BL);
        if(error_flag==1){
            qDebug()<<"reading failed";
            exit(EXIT_FAILURE);
        }
        usleep(8000);
        if(error_flag==0){
            //dlgPCD->setModal(true);
            DialogPCD *dlgPCD = new DialogPCD(this);
            bool a=dlgPCD->setbalance();
            while (a==false);
            dlgPCD->show();
        }

     }
}

void MyWindow::on_B_PICC_clicked()//picc
{
    C_mode=PICC;
    DialogPICC *dlgPICC = new DialogPICC(this);

    dlgPICC->setModal(true);
    dlgPICC->show();
}

void MyWindow::on_B_deafault_clicked() //default
{
    C_RegPara=Default;
    QString msg="You have chose Defalut Parameter";
    historylog.append(msg);
    rebuildHistory();
}

void MyWindow::on_B_user_clicked() //user para
{
    C_RegPara=User;
    QString msg="You have chose your setted Parameter";
    historylog.append(msg);
    QString msg1="Now Start to Configure...";
    historylog.append(msg1);
    rebuildHistory();
    //rebuildHistory();
    //sleep(1);
    emit emitSetReg();

}
void MyWindow::setReg()
{
    reg_info CIU_RFCfg = {
                "CIU_RFCfg",
                0x63,
                0x16,
                0x74
            };
    reg_info CIU_ModWidth={
                "CIU_ModWidth",
                0x63,
                0x14,
                0x28
            };
    reg_info CIU_TxBitPhase={
                "CIU_TxBitPhase",
                0x63,
                0x15,
                0x8a
            };
    change_reg(CIU_RFCfg);
    sleep(1);
    change_reg(CIU_ModWidth);
    sleep(1);
    change_reg(CIU_TxBitPhase);
    QString msg2="Done...\n";
    historylog.append(msg2);
    rebuildHistory();
}
