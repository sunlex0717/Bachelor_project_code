#ifndef DIALOGPCD_H
#define DIALOGPCD_H

#include <QDialog>
#include <QStringList>
#include <QString>
namespace Ui {
class DialogPCD;
}

class DialogPCD : public QDialog
{
    Q_OBJECT
    static const int BL=2;
public:
    explicit DialogPCD(QWidget *parent = 0);
    ~DialogPCD();
    void rebuildHistory();
    bool ReCharge();
    bool Consume();
    bool setbalance();
    void amt_bal_to_balance();
    void balance_to_amt_bal();
    //void setmode(int mode) {C_mode=mode;}
    //void setRegP(int para) {C_RegPara=para;}
private slots:
    void on_Confirm_RC_clicked();

    void on_Confirm_CM_clicked();

private:
    Ui::DialogPCD *ui;
    QStringList PCD_history;

    double amt_recharge;
    double amt_consume;
    char balance[7];
    double amt_bal;
    //int C_mode; //0:PCD 1:PICC
    //int C_RegPara; //0:default 1:user
};

#endif // DIALOGPCD_H
