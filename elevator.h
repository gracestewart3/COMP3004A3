#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "door.h"
#include "floorsensor.h"
#include "announcementsystem.h"
#include "bell.h"

#include <string>
using namespace std;

class Elevator{
public:
    Elevator(int, int=1, string="idle");
    ~Elevator();

private:
    int id;
    int currFloor;
    string currState;
    Door* door;
    FloorSensor* floorSensor;
    AnnouncementSystem* announcementSystem;
    Bell* bell;
};

#endif // ELEVATOR_H
