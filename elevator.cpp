#include "elevator.h"

Elevator::Elevator(int elevatorId, int floor, string state, string dir, string goalDir){
    id = elevatorId;
    currFloor = floor;
    currState =  state;
    direction = dir;
    goalDirection = goalDir;
    Door* door = new Door;
    FloorSensor* floorSensor = new FloorSensor;
    AnnouncementSystem* announcementSystem = new AnnouncementSystem;
    Bell* bell = new Bell;
}

Elevator::~Elevator(){
    delete door;
    delete floorSensor;
    delete announcementSystem;
    delete bell;
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
