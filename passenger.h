#ifndef PASSENGER_H
#define PASSENGER_H
#include "behaviour.h"
#include "elevator.h"
#include <QObject>

class Passenger : public QObject {
    Q_OBJECT
    public:
        Passenger(int, int, int, Behaviour**, int, string, int=nextId, bool=false, Elevator* =nullptr);
        ~Passenger();
        int startingFloor;
        int floorRequestTimeStep;
        int destination;
        int id;
        Behaviour** behaviours;
        int numBehaviours;
        string direction;
        static int nextId;
        bool isInElevator;
        Elevator* inElevator;
        void boardElevator(Elevator *);
        void disembarkElevator();
        void pushDestinationButton();

};

#endif // PASSENGER_H
