#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "door.h"

#include <string>
#include <set>
using namespace std;

class Elevator{
public:
    Elevator(int, int=1, string="idle", string="none", string="none", bool=false, int=-1);
    ~Elevator();
    int id;
    int currFloor;
    string currState;
    string direction;
    string goalDirection;
    Door* door;
    set<int> requests;
    bool recalled;
    int recalledFloor;
    void enqueueRequest(int);
    void serviceRequest(int);
    int closestRequest();
    void recallToFloor(int);
};

#endif // ELEVATOR_H
