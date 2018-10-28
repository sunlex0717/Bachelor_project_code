#include "dialogpicc.h"
#include "ui_dialogpicc.h"

DialogPICC::DialogPICC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPICC)
{
    ui->setupUi(this);
}

DialogPICC::~DialogPICC()
{
    delete ui;
}
