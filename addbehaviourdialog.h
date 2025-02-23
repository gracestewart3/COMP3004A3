#ifndef ADDBEHAVIOURDIALOG_H
#define ADDBEHAVIOURDIALOG_H

#include <QDialog>

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
};

#endif // ADDBEHAVIOURDIALOG_H
