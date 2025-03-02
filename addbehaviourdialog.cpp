#include "addbehaviourdialog.h"
#include "ui_addbehaviourdialog.h"
#include <QPushButton>

AddBehaviourDialog::AddBehaviourDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBehaviourDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Add");
    connect(ui->button_pressed, &QComboBox::currentTextChanged, this, &AddBehaviourDialog::onComboBoxTextChanged);
}

AddBehaviourDialog::~AddBehaviourDialog()
{
    delete ui;
}

void AddBehaviourDialog::on_buttonBox_accepted(){
    std::string button;
    bool isFloor;

    if (!(ui->floor->text()).isEmpty()){
        button = (ui->floor->text()).toStdString();
        isFloor = true;
    }
    else{
        button = (ui->button_pressed->currentText()).toStdString();
        isFloor = false;
    }

    int timestep = (ui->timestep_of_press->text()).toInt();

    emit updatePassenger(button.c_str(),timestep, isFloor);

    this->close();
}

void AddBehaviourDialog::onComboBoxTextChanged(const QString text) {
        if (text == "Destination Button") {
            ui->floor->show();
            ui->floor_label->show();
        }
        else {
            ui->floor->hide();
            ui->floor->setText(QString(""));
            ui->floor_label->hide();
        }
}
