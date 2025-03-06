/*
 CLASS NAME: Door
 PURPOSE: To represent the doors of a specific elevator in the simulation
*/
#ifndef DOOR_H
#define DOOR_H
#include <string>
using namespace std;

class Door
{
public:
    Door(bool=false);
    bool doorOpen;
    int numDoorEvents;
    void openDoor();
    void closeDoor();

};

#endif // DOOR_H
