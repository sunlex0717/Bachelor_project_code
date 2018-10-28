#include "mywindow.h"
#include <QApplication>
#include <dialoglogin.h>
#include <dialogpcd.h>
#include <dialogpicc.h>

#include <QString>
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

unchar sdata[SIZE];   //ra // complete
unchar cdata[SIZE];   //rb // raw
unchar payload[SIZE];  //rc//show
unchar rdata[SIZE];  //rd// recieve

unint ra,rb,rc,rd,i,j;
unint sum; //TFI+... ...+PDn
unchar state_response;
unint state_ack;
unint error_flag;
unchar Tg;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DialogLogin dlgLI;
    MyWindow w(0,dlgLI.getUserName());
    if(dlgLI.exec()==QDialog::Accepted){
        w.show();
        return a.exec();
    }
    else{
        a.closingDown();
    }

}
