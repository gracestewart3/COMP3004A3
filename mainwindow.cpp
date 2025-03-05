#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->main_stack->setCurrentIndex(1);
    ui->passengers_added_label->hide();
    ui->events_added_label->hide();
    ui->pause_btn->hide();
    ui->stop_btn->hide();


    numPassengers = 0;
    passengers = new Passenger*[MAX_ARR];
    numEvents = 0;
    events = new SafetyEvent*[MAX_ARR];
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

void MainWindow::handleNewPassenger(int floor, int time, int dest, Behaviour** behaviours, int  numActions, string direction){
    Passenger* newPassenger = new Passenger(floor, time, dest, behaviours, numActions, direction);

    passengers[numPassengers] = newPassenger;
    numPassengers++;

    ui->passengers_added_label->show();

    QString message;
    message = QString::asprintf("Passenger %d:\nStarting on floor %d at time %d, going %s to floor %d.\n", newPassenger->id, floor, time, direction.c_str(), dest);
    ui->passenger_display->setText(ui->passenger_display->text() + "\n" + message);

}
void MainWindow::handleNewEvent(string event, int time, bool isElevatorSpecific, int id){
    SafetyEvent* newEvent = new SafetyEvent(event, time, isElevatorSpecific, id);

    events[numEvents] = newEvent;
    numEvents++;

    string event_str;
    if (isElevatorSpecific){
        event_str = "on elevator " + to_string(id) + ", ";
    }
    else{
        event_str = "";
    }

     ui->events_added_label->show();

    QString message;
    message = QString::asprintf("%s event %sat timestep %d.\n", event.c_str(), event_str.c_str(), time);
    ui->event_display->setText(ui->event_display->text() + "\n" + message);
}

void MainWindow::on_start_btn_clicked(){//eventually seperate the behaviour for initial start and resume
    SimulationController* controller = new SimulationController(events, numEvents, passengers, numPassengers, ui->num_elevators->text().toInt(), ui->num_floors->text().toInt());
    QObject::connect(controller, &SimulationController::updateTimestep, this, &MainWindow::handleNewTimestep);
    QObject::connect(controller, &SimulationController::updateLog, this, &MainWindow::handleNewLog);
    ui->main_stack->setCurrentIndex(0);
    ui->pause_btn->show();
    ui->stop_btn->show();
    for(int i=ui->num_elevators->text().toInt(); i>0;i--){
        addElevator();
    }

    QtConcurrent::run([controller]() {//run concurrently so that it can send signals
        controller->runSimulation();
    });


}

void MainWindow::addElevator(){

    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(ui->elevator_frame->layout());

    QString elevatorText = tr("Elevator #%1").arg(layout->count());

    QPushButton* button = new QPushButton(elevatorText, ui->elevator_frame);

    layout->insertWidget(0, button);

}

void MainWindow::handleNewTimestep(int time){
    ui->timestep->display(time);
}

void MainWindow::handleNewLog(QString txt){
    ui->log_console->append(txt);
}
