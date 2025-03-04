#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include "passenger.h"
#include "safetyevent.h"
#include "elevator.h"
#include "floor.h"
#include <QObject>
#include <QtDebug>
#include <cmath>
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
        int selectElevator(int);
        void informElevatorOfRequest(int, int);

};

#endif // SIMULATIONCONTROLLER_H
