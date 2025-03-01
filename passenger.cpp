#include "passenger.h"

int Passenger::nextId = 0;

Passenger::Passenger(int floor, int time, Behaviour** actions, string dir, int i){
    startingFloor = floor;
    floorRequestTimeStep = time;
    behaviours = actions;
    direction = dir;
    id = i;
    nextId++;
}
