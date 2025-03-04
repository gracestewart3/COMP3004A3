#include "passenger.h"

int Passenger::nextId = 0;

Passenger::Passenger(int floor, int time, int dest, Behaviour** actions, int numActions, string dir, int i, bool isInside, Elevator* elevator){
    startingFloor = floor;
    floorRequestTimeStep = time;
    behaviours = actions;
    destination = dest;
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

void Passenger::boardElevator(Elevator* elevator){
    isInElevator = true;
    inElevator = elevator;
}
