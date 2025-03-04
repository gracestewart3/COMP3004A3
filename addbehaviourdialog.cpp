#include "addbehaviourdialog.h"
#include "ui_addbehaviourdialog.h"
#include <QPushButton>

AddBehaviourDialog::AddBehaviourDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBehaviourDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Add");
}

AddBehaviourDialog::~AddBehaviourDialog()
{
    delete ui;
}

void AddBehaviourDialog::on_buttonBox_accepted(){
    std::string button = (ui->button_pressed->currentText()).toStdString();

    int timestep = (ui->timestep_of_press->text()).toInt();

    emit updatePassenger(button.c_str(),timestep);

    this->close();
}


