#include "passenger.h"

int Passenger::nextId = 0;

Passenger::Passenger(int floor, int time, Behaviour** actions, int numActions, string dir, int i, bool isInside, int elevator){
    startingFloor = floor;
    floorRequestTimeStep = time;
    behaviours = actions;
    direction = dir;
    id = i;
    nextId++;
    isInElevator = isInside;
    inElevator = elevator;
    numBehaviours = numActions;
}

Passenger::~Passenger(){
    for(int i = 0; i < numBehaviours; i++){
        delete behaviours[i];
    }
    delete [] behaviours;
}
