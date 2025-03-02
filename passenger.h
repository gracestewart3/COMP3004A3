#ifndef PASSENGER_H
#define PASSENGER_H
#include "behaviour.h"

class Passenger
{
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
};

#endif // PASSENGER_H
