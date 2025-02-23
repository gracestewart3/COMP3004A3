#ifndef ADDPASSENGERDIALOG_H
#define ADDPASSENGERDIALOG_H

#include <QDialog>
#include "addbehaviourdialog.h"

namespace Ui {
class AddPassengerDialog;
}

class AddPassengerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPassengerDialog(QWidget *parent = nullptr);
    ~AddPassengerDialog();

private:
    Ui::AddPassengerDialog *ui;
    AddBehaviourDialog *behaviourWindow;

private slots:
    void on_add_behaviour_clicked();
    void on_buttonBox_accepted();
};

#endif // ADDPASSENGERDIALOG_H
