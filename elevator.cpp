#include "elevator.h"

Elevator::Elevator(int elevatorId, int floor, string state, string dir, string goalDir, bool recall, int recFlr){
    id = elevatorId;
    currFloor = floor;
    currState =  state;
    direction = dir;
    goalDirection = goalDir;
    recalled=recall;
    recalledFloor=recFlr;
    door = new Door();
}

Elevator::~Elevator(){
    delete door;
}

void Elevator::enqueueRequest(int floor){
    requests.insert(floor);
}

void Elevator::serviceRequest(int floor){
    requests.erase(floor);
}

int Elevator::closestRequest(){
    int closest = *requests.begin();
    std::set<int>::iterator it;
    for (it = requests.begin(); it != requests.end(); ++it) {
        int f = *it;
        if(abs(f-currFloor) <= abs(closest-currFloor)){
            closest = f;
        }
    }
    return closest;
}

void Elevator::recallToFloor(int floor){
    //first, empty current request queue...
    requests.clear();
    //insert the safe floor to the queue...
    requests.insert(floor);
    //stop current motion...
    direction="none";
    goalDirection="none";
    recalled="true";
    recalledFloor = floor;
}
