#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_event_clicked()
{
    eventWindow = new AddEventDialog(this);
    QObject::connect(eventWindow, &AddEventDialog::completeEvent, this, &MainWindow::handleNewEvent);
    eventWindow->show();

}

void MainWindow::on_add_passenger_clicked()
{
    passengerWindow = new AddPassengerDialog(this);
    QObject::connect(passengerWindow, &AddPassengerDialog::completePassenger, this, &MainWindow::handleNewPassenger);
    passengerWindow->show();

}

void MainWindow::handleNewPassenger(int floor, int time, Behaviour** behaviours, string direction){
    Passenger* newPassenger = new Passenger(floor, time, behaviours,  direction);

    //eventually add to some sort of list of passengers.....

    QString message;
    message = QString::asprintf("Passenger %d:\nStarting on floor %d at time %d, going %s.\n", newPassenger->id, floor, time, direction.c_str());
    ui->passenger_display->setText(ui->passenger_display->text() + "\n" + message);
}
void MainWindow::handleNewEvent(string event, int time, bool isElevatorSpecific, int id){
    SafetyEvent* newEvent = new SafetyEvent(event, time, isElevatorSpecific, id);

    //eventually add to some sort of list of events.....
    string event_str;
    if (isElevatorSpecific){
        event_str = "on elevator " + to_string(id) + ", ";
    }
    else{
        event_str = "";
    }

    QString message;
    message = QString::asprintf("%s event %sat timestep %d.\n", event.c_str(), event_str.c_str(), time);
    ui->event_display->setText(ui->event_display->text() + "\n" + message);
}
