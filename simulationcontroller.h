#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "passenger.h"
#include "safetyevent.h"
#include "elevator.h"
#include "floor.h"
#include <QObject>
#include <QString>
#include <QtDebug>
#include <cmath>
#include <unistd.h>
#define MAX_ARR       24

class SimulationController : public QObject {

    Q_OBJECT

    public:
        SimulationController(SafetyEvent**, int, Passenger**, int, int, int, bool=false);
        ~SimulationController();
        void runSimulation();
    private:
        bool simulationIsComplete;
        int numElevators;
        int numFloors;
        int numPassengers;
        int numEvents;
        int numActivePassengers;
        int* activePassengers; //indexes/ids of passengers with >= 1 behaviour with a future timestep
        int* futureEvents; //indexes of events with a future timestep
        int numFutureEvents;
        Elevator** elevators;
        Passenger** passengers;
        SafetyEvent** events;
        Floor** floors;
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
signals:
    void updateTimestep(int);
    void updateLog(QString);
    void updateSafetyEvents(QString);
};

#endif // SIMULATIONCONTROLLER_H
