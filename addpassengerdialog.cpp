#include "addpassengerdialog.h"
#include "ui_addpassengerdialog.h"

AddPassengerDialog::AddPassengerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPassengerDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Complete");
}

AddPassengerDialog::~AddPassengerDialog()
{
    delete ui;
}

void AddPassengerDialog::on_add_behaviour_clicked()
{
    behaviourWindow = new AddBehaviourDialog(this);
    behaviourWindow->show();

}

void AddPassengerDialog::on_buttonBox_accepted(){
    qInfo("Completed passenger!"); //change this out for something useful that actually creates a passenger
    this->close();
}
