#ifndef ADDPASSENGERDIALOG_H
#define ADDPASSENGERDIALOG_H

#include <QDialog>
#include "addbehaviourdialog.h"
#include "behaviour.h"

#define MAX_ARR       24

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
    Behaviour** behaviours;
    int numBehaviours;

private slots:
    void on_add_behaviour_clicked();
    void on_buttonBox_accepted();

public slots:
    void handleNewBehaviour(const char*, int);

signals:
    void completePassenger(int, int, int, Behaviour**, int, string);
};

#endif // ADDPASSENGERDIALOG_H
