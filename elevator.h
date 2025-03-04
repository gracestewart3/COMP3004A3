#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "door.h"
#include "floorsensor.h"
#include "announcementsystem.h"
#include "bell.h"

#include <string>
#include <set>
using namespace std;

class Elevator{
public:
    Elevator(int, int=1, string="idle", string="none");
    ~Elevator();
    int id;
    int currFloor;
    string currState;
    string direction;
    Door* door;
    FloorSensor* floorSensor;
    AnnouncementSystem* announcementSystem;
    Bell* bell;
    set<int> requests;
    void enqueueRequest(int);
    void serviceRequest(int);
    int closestRequest();
};

#endif // ELEVATOR_H
