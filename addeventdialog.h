#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEventDialog(QWidget *parent = nullptr);
    ~AddEventDialog();

private:
    Ui::AddEventDialog *ui;
};

#endif // ADDEVENTDIALOG_H
