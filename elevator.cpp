#include "elevator.h"

Elevator::Elevator(int elevatorId, int floor, string state){
    id = elevatorId;
    currFloor = floor;
    currState =  state;
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
