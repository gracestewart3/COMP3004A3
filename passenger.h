#ifndef PASSENGER_H
#define PASSENGER_H
#include "behaviour.h"

class Passenger
{
public:
    Passenger(int, int, Behaviour**, string, int=nextId);
    int startingFloor;
    int floorRequestTimeStep;
    int id;
    Behaviour** behaviours;
    string direction;
    static int nextId;
};

#endif // PASSENGER_H
