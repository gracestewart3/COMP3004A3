/*
 CLASS NAME: AddEventDialog
 PURPOSE: To provide the user with a GUI to add safety events (eg. fire alarm)
*/
#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <string>
using namespace std;

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

private slots:
    void onComboBoxTextChanged(const QString text);
    void on_buttonBox_accepted();

signals:
    void completeEvent(string, int, bool, int);
};

#endif // ADDEVENTDIALOG_H
