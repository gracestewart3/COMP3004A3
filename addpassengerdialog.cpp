#include "addpassengerdialog.h"
#include "ui_addpassengerdialog.h"

AddPassengerDialog::AddPassengerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPassengerDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Complete");
    numBehaviours = 0;
    behaviours = new Behaviour*[MAX_ARR];

}

AddPassengerDialog::~AddPassengerDialog()
{
    delete ui;
}

void AddPassengerDialog::on_add_behaviour_clicked()
{
    behaviourWindow = new AddBehaviourDialog(this);
    QObject::connect(behaviourWindow, &AddBehaviourDialog::updatePassenger, this, &AddPassengerDialog::handleNewBehaviour);
    behaviourWindow->show();

    behaviourWindow = new AddBehaviourDialog(this);


}

void AddPassengerDialog::on_buttonBox_accepted(){
    emit completePassenger(ui->startingFloor->text().toInt(),ui->timestep->text().toInt(), behaviours, numBehaviours, ui->direction->currentText().toStdString());
    this->close();
}


void AddPassengerDialog::handleNewBehaviour(const char* button_pressed, int timestep, bool isFloor) {
    const char* button_prefix;
    if (isFloor){
        button_prefix = "Floor ";
    }
    else{
        button_prefix = "";
    }

    Behaviour* behaviour = new Behaviour(timestep, button_pressed);
    behaviours[numBehaviours] = behaviour;
    numBehaviours++;

    QString message;
    message = QString::asprintf("Button: %s%s, Timestep: %d", button_prefix, button_pressed, timestep);
    ui->behaviour_display->setText(ui->behaviour_display->text() + "\n" + message);
}
