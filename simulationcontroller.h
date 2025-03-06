#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "passenger.h"
#include "safetyevent.h"
#include "elevator.h"
#include <QObject>
#include <QString>
#include <QtDebug>
#include <cmath>
#include <unistd.h>
#define MAX_ARR       24

class SimulationController : public QObject {

    Q_OBJECT

    public:
        SimulationController(SafetyEvent**, int, Passenger**, int, int, int);
        ~SimulationController();
        void runSimulation();

    private:
        int numElevators;
        int numFloors;
        int numPassengers;
        int numEvents;

        int numActivePassengers;
        int* activePassengers;
        int* futureEvents;
        int numFutureEvents;
        bool isPaused;
        bool isStopped;

        Elevator** elevators;
        Passenger** passengers;
        SafetyEvent** events;
        void removePassengerFromActive(int);
        void removeEventFromFuture(int);
        int selectElevator(int, string);
        void informElevatorOfRequest(int, int, string);
        void onboardElevator(Elevator*, string*);
        void unloadElevator(Elevator*, string*);
        void respondToBuildingFireAlarm(string *);
        void respondToElevatorFireAlarm(string *, int);
        void respondToDoorObstruction(string *, int);
        void respondToHelpSignal(string *, int, int);
        void respondToDoorOpen(string *, int, int);
        void respondToDoorClose(string *, int, int);
        void respondToOverload(string *, int);
        void respondToPowerOutage(string *);
        string getElevatorStr();

    public slots:
        void pause();
        void resume();
        void stop();

    signals:
        void updateTimestep(int);
        void updateLog(QString);
        void updateSafetyEvents(QString);
        void updateElevators(QString);
        void endSimulation();
};

#endif // SIMULATIONCONTROLLER_H
