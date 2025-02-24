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
    int floor_str = (ui->floor->text()).toInt();
    int timestep = (ui->timestep_of_press->text()).toInt();
    std::string button = (ui->button_pressed->currentText()).toStdString();
    qInfo("Button: %s, Timestep: %d, Floor: %d", button.c_str(), timestep, floor_str);
    qInfo("Completed behaviour!"); //change this out for something useful that actually creates a behaviour + updates display
    this->close();
}
