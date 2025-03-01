#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);
    connect(ui->safety_event, &QComboBox::currentTextChanged, this, &AddEventDialog::onComboBoxTextChanged);

}

AddEventDialog::~AddEventDialog()
{
    delete ui;
}

void AddEventDialog::on_buttonBox_accepted(){
    qInfo("Completed event!");

    bool isElevatorSpecific;
    if (!(ui->elevator_id->text()).isEmpty()){
        isElevatorSpecific = true;
    }
    else{
        isElevatorSpecific = false;
    }
    emit completeEvent(ui->safety_event->currentText().toStdString(), ui->timestep->text().toInt(), isElevatorSpecific, ui->elevator_id->text().toInt());
    this->close();
}

void AddEventDialog::onComboBoxTextChanged(const QString text) {
        if (text != "Fire Alarm (from building)" and text!="Power Outage") {
            ui->elevator_id->show();
            ui->elevator_id_label->show();
        }
        else {
            ui->elevator_id->hide();
            ui->elevator_id->setText(QString(""));
            ui->elevator_id_label->hide();
        }
}
