#ifndef ADDBEHAVIOURDIALOG_H
#define ADDBEHAVIOURDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class AddBehaviourDialog;
}

class AddBehaviourDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBehaviourDialog(QWidget *parent = nullptr);
    ~AddBehaviourDialog();

private:
    Ui::AddBehaviourDialog *ui;

private slots:
    void on_buttonBox_accepted();

signals:
    void updatePassenger(const char*, int);
};

#endif // ADDBEHAVIOURDIALOG_H
