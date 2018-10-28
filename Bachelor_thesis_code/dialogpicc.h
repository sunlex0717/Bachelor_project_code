#ifndef DIALOGPICC_H
#define DIALOGPICC_H

#include <QDialog>

namespace Ui {
class DialogPICC;
}

class DialogPICC : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPICC(QWidget *parent = 0);
    ~DialogPICC();

private:
    Ui::DialogPICC *ui;
};

#endif // DIALOGPICC_H
