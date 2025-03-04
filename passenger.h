#ifndef PASSENGER_H
#define PASSENGER_H
#include "behaviour.h"
#include <QObject>

class Passenger : public QObject {
    Q_OBJECT
    public:
        Passenger(int, int, Behaviour**, int, string, int=nextId, bool=false, int=-1);
        ~Passenger();
        int startingFloor;
        int floorRequestTimeStep;
        int id;
        Behaviour** behaviours;
        int numBehaviours;
        string direction;
        static int nextId;
        bool isInElevator;
        int inElevator;

    //signals:
        //void pressFloorButton(int, string);
};

#endif // PASSENGER_H
