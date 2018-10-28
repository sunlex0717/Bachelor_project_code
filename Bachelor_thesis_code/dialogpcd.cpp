#include "dialogpcd.h"
#include "ui_dialogpcd.h"
#include "mywindow.h"
#include <QString>
#include <QStringList>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
extern "C"{
#include "funct.h"
#include "ser.h"
}
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
DialogPCD::DialogPCD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPCD)
{
    ui->setupUi(this);
    ui->amout_CM->setMaximum(1000.00);
    ui->amout_CM->setMinimum(0.00);
    ui->amout_CM->setSingleStep(10.00);

    ui->amout_RC->setMaximum(1000.00);
    ui->amout_RC->setMinimum(0.00);
    ui->amout_RC->setSingleStep(10.00);
    QString msg="Welcome!";
    PCD_history.append(msg);
    rebuildHistory();
}

DialogPCD::~DialogPCD()
{
    delete ui;
}

void DialogPCD::rebuildHistory()
{
    QString history=PCD_history.join(QLatin1String("\n" ));
    ui->PCD_msg->setText(history);
}

bool DialogPCD::setbalance()//16 bytes ONLY Highest 6 bytes are needed
{
    for(int i=0;i<7;i++){
        balance[i]=payload[i+3];
    }
    QString Q_bal;
    balance_to_amt_bal();
    amt_bal_to_balance();
    Q_bal.append(balance);
    QString msg="Your balance is:";
    QString msg1=msg+Q_bal;
    PCD_history.append(msg1);
    rebuildHistory();
    qDebug()<<balance;
    balance_to_amt_bal();
    //amt_bal_to_balance();
    return true;
}

bool DialogPCD::ReCharge()
{
    amt_bal+=amt_recharge;
    amt_bal_to_balance();
    error_flag=writing(BL,balance);
    if(error_flag==0)
        return true;
    else
        return false;
}


bool DialogPCD::Consume()
{
    amt_bal-=amt_consume;
    amt_bal_to_balance();
    error_flag=writing(BL,balance);
    if(error_flag==0)
        return true;
    else
        return false;
}

void DialogPCD::amt_bal_to_balance()
{
    char temp[6];
    int temp_bal=amt_bal*100;
    temp[0]=(char)(temp_bal/100000);
    temp[1]=(char)((temp_bal-temp[0]*100000)/10000);
    temp[2]=(char)((temp_bal-temp[0]*100000-temp[1]*10000)/1000);
    temp[3]=(char)((temp_bal-temp[0]*100000-temp[1]*10000-temp[2]*1000)/100);
    temp[4]=(char)((temp_bal-temp[0]*100000-temp[1]*10000-temp[2]*1000-temp[3]*100)/10);
    temp[5]=(char)((temp_bal-temp[0]*100000-temp[1]*10000-temp[2]*1000-temp[3]*100-temp[4]*10));

    for(int i=0;i<4;i++){
        balance[i]=temp[i]+0x30;
    }
    balance[4]=0x2e;
    balance[5]=temp[4]+0x30;
    balance[6]=temp[5]+0x30;
    qDebug()<<"amt_bal_to_balance:"<<balance;
}

void DialogPCD::balance_to_amt_bal()
{
    int temp[6];
    for(int i=0;i<4;i++){
        temp[i]=balance[i]-0x30;
    }
    temp[4]=balance[5]-0x30;
    temp[5]=balance[6]-0x30;
    amt_bal=temp[0]*1000+temp[1]*100+temp[2]*10+temp[3]*1
                    +temp[4]*0.1+temp[5]*0.01;
    qDebug()<<"balance_to_amt_bal:"<<amt_bal;
}



void DialogPCD::on_Confirm_RC_clicked() //get the number of ReCharge
{
    amt_recharge=ui->amout_RC->value();
    //amt_bal+=amt_recharge;
    //amt_bal_to_balance();
    ReCharge();
    QString Q_bal;
    Q_bal.append(balance);
    QString msg="Now your balance is: ";
    QString msg1=msg+Q_bal;
    PCD_history.append(msg1);
    rebuildHistory();
}


void DialogPCD::on_Confirm_CM_clicked()
{
    amt_consume=ui->amout_CM->value();
    if(amt_bal<amt_consume)
    {
        QString msg="Your balance is not sufficient,please ReCharge first!";
        PCD_history.append(msg);
        rebuildHistory();
    }
    else
    {
        //amt_bal-=amt_consume;
        //amt_bal_to_balance();
        Consume();
        QString Q_bal;
        Q_bal.append(balance);
        QString msg="Now your balance is: ";
        QString msg1=msg+Q_bal;
        PCD_history.append(msg1);
        rebuildHistory();
    }
}
