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
    simulationStarted = false;
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

void MainWindow::on_start_btn_clicked(){
    if(!simulationStarted){
        SimulationController* controller = new SimulationController(events, numEvents, passengers, numPassengers, ui->num_elevators->text().toInt(), ui->num_floors->text().toInt());
        QObject::connect(controller, &SimulationController::updateTimestep, this, &MainWindow::handleNewTimestep);
        QObject::connect(controller, &SimulationController::updateLog, this, &MainWindow::handleNewLog);
        QObject::connect(controller, &SimulationController::updateSafetyEvents, this, &MainWindow::handleNewSafetyEvents);
        QObject::connect(controller, &SimulationController::updateElevators, this, &MainWindow::handleNewElevatorStr);
        QObject::connect(this, &MainWindow::pauseSimulation, controller, &SimulationController::pause);
        QObject::connect(this, &MainWindow::resumeSimulation, controller, &SimulationController::resume);
        QObject::connect(this, &MainWindow::stopSimulation, controller, &SimulationController::stop);
        ui->main_stack->setCurrentIndex(0);
        ui->pause_btn->show();
        ui->stop_btn->show();


        QtConcurrent::run([controller]() {//run concurrently so that it can send signals
            controller->runSimulation();
        });
        simulationStarted = true;
    }
    else{
        ui->pause_btn->setChecked(false);
        ui->start_btn->setChecked(true);
        emit resumeSimulation();
    }
}

void MainWindow::on_pause_btn_clicked(){
    ui->pause_btn->setChecked(true);
    ui->start_btn->setChecked(false);
    emit pauseSimulation();
}

void MainWindow::on_stop_btn_clicked(){
    ui->stop_btn->setChecked(true);
    ui->start_btn->setChecked(false);
    ui->pause_btn->setChecked(false);

    emit stopSimulation();

}



void MainWindow::handleNewTimestep(int time){
    ui->timestep->display(time);
}

void MainWindow::handleNewLog(QString txt){
    ui->log_console->append(txt);
}

void MainWindow::handleNewSafetyEvents(QString txt){
    ui->active_safety_events->setText(txt);
}

void MainWindow::handleNewElevatorStr(QString txt){
    ui->elevators->setText(txt);
}

void MainWindow::handleEndSimulation() {
    ui->stop_btn->setEnabled(false);
    ui->start_btn->setEnabled(false);
    ui->pause_btn->setEnabled(false);
    QObject* senderObj = sender();
    if (senderObj) {
        senderObj->deleteLater();
    }
}
