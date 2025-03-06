#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addeventdialog.h"
#include "addpassengerdialog.h"
#include "passenger.h"
#include "safetyevent.h"
#include "simulationcontroller.h"
#include <QDebug>
#include <QString>
#include <QtConcurrent/QtConcurrent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_event_clicked();
    void on_add_passenger_clicked();
    void on_start_btn_clicked();
    void on_pause_btn_clicked();
    void on_stop_btn_clicked();

private:
    Ui::MainWindow *ui;
    AddEventDialog *eventWindow;
    AddPassengerDialog *passengerWindow;
    Passenger** passengers;
    int numPassengers;
    SafetyEvent** events;
    int numEvents;
    bool simulationStarted;
    void addElevator();

public slots:
    void handleNewPassenger(int, int, int, Behaviour**, int, string);
    void handleNewTimestep(int);
    void handleNewLog(QString);
    void handleNewSafetyEvents(QString);
    void handleNewEvent(string, int, bool, int);
    void handleEndSimulation();
    void handleNewElevatorStr(QString);

signals:
    void pauseSimulation();
    void resumeSimulation();
    void stopSimulation();

};
#endif // MAINWINDOW_H
