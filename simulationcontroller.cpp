#include "simulationcontroller.h"

SimulationController::SimulationController(SafetyEvent** evs, int numEvs, Passenger** pgs, int numPgs, int numEls, int numFls, bool complete){
    simulationIsComplete = complete;
    numElevators = numEls;
    elevators = new Elevator*[numElevators];
    for(int i = 0; i < numElevators; i++){
        elevators[i] = new Elevator(i);
    }

    //Might make more sense not to have individual floors? Like have one instance of "Floors"??????
    numFloors = numFls;
    floors = new Floor*[numFloors];
    for(int i = 0; i < numFloors; i++){
        floors[i] = new Floor(i+1);
    }

    events = evs;
    numEvents = numEvs;
    numFutureEvents = 0;
    futureEvents = new int[numEvents];
    for(int i = 0; i < numEvents; i++){
        futureEvents[i] = i;
        numFutureEvents++;
    }

    passengers = pgs;
    numPassengers = numPgs;
    numActivePassengers=0;
    activePassengers = new int[numPassengers];
    for(int i = 0; i < numPassengers; i++){
        activePassengers[i] = i;
        numActivePassengers++;
    }



}

SimulationController::~SimulationController(){
    delete [] activePassengers;
    delete [] futureEvents;

    for(int i = 0; i < numElevators; i++){
        delete elevators[i];
    }
    delete [] elevators;

    for(int i = 0; i < numFloors; i++){
        delete floors[i];
    }
    delete [] floors;

    for(int i = 0; i < numPassengers; i++){
        delete passengers[i];
    }
    delete [] passengers;

    for(int i = 0; i < numEvents; i++){
        delete events[i];
    }
    delete [] events;

}

void SimulationController::removePassengerFromActive(int index){
    int newSize = numActivePassengers-1;
    if (newSize <= 0){
        delete [] activePassengers;
        activePassengers = nullptr;
        numActivePassengers = 0;
        qDebug() << "removed passenger " << passengers[index]->id << " from active (last passenger)";
        return;
    }

    int*  temp = new int[newSize];
    int idx = 0;
    for(int i = 0; i < newSize+1; i++){
        if (activePassengers[i] != index){
            temp[idx] = activePassengers[i];
            idx++;
        }
    }
    delete [] activePassengers;
    activePassengers = temp;
    numActivePassengers--;
    qDebug() << "removed passenger " << passengers[index]->id << " from active";

}

void SimulationController::removeEventFromFuture(int index){
    int newSize = numFutureEvents-1;
    if (newSize <= 0){
        delete [] futureEvents;
        futureEvents = nullptr;
        numFutureEvents = 0;
        qDebug() << "removed event" << events[index]->type.c_str() << "at timestep" << events[index]->timestep << " from future events (last event)";
        return;
    }

    int*  temp = new int[newSize];
    int idx = 0;
    for(int i = 0; i < newSize+1; i++){
        if (futureEvents[i] != index){
            temp[idx] = futureEvents[i];
            idx++;
        }
    }
    delete [] futureEvents;
    futureEvents = temp;
    numFutureEvents--;
    qDebug() << "removed event" << events[index]->type.c_str() << "at timestep" << events[index]->timestep << " from future events";

}

int SimulationController::selectElevator(int flr){//now I'm thinking this should take a direction too? Like the "goal"' direction?
    int best = 0;
    for(int i = 0; i < numElevators; i++){
        int oldDistance = abs(elevators[best]->currFloor - flr);
        int newDistance = abs(elevators[i]->currFloor - flr);

        bool newRightDirection = (elevators[i]->currFloor >= flr && elevators[i]->direction=="down") || (elevators[i]->currFloor <= flr and elevators[i]->direction=="up");

        if(((newDistance <= oldDistance) && newRightDirection) || ((newDistance <= oldDistance) && elevators[i]->direction=="none")){
            best = i;
        }
    }
    return best;
}

void SimulationController::informElevatorOfRequest(int el, int flr){
    elevators[el]->enqueueRequest(flr);
}


void SimulationController::runSimulation(){
    //notes: Should probably split into smaller functions. Still need to add support for car request, and how to keep track of what passengers are where
    int timestep = 0;
    while (timestep  <= 10){ //change to stop  when all events are dealt  with
        qDebug() << timestep;
        for(int k = 0; k < numActivePassengers; k++){
            int i = activePassengers[k];
            bool pIsActive = false;
            if (passengers[i]->floorRequestTimeStep == timestep){
                //note: right now I am bypassing floors all together. Mention this in assignment
                int startingFloor = passengers[i]->startingFloor;
                int assignedElevator = selectElevator(startingFloor);
                informElevatorOfRequest(assignedElevator, startingFloor);
                qDebug() << "Deal with inital floor request for passenger " << passengers[i]->id << "...";

            }
            if (passengers[i]->floorRequestTimeStep > timestep || !passengers[i]->isInElevator){
                pIsActive = true;
            }
            for(int j = 0; j < passengers[i]->numBehaviours; j++){

                if (passengers[i]->behaviours[j]->getTimestep() == timestep){
                    string btn = passengers[i]->behaviours[j]->getButton();
                    if(btn == "Help"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the help button";
                         //eventually do something useful here........................................
                    }
                    else if(btn == "Close Door"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the close door button";
                         //eventually do something useful here........................................
                    }
                    else if(btn == "Open Door"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the open door button";
                         //eventually do something useful here........................................
                    }

                }
                else if (passengers[i]->behaviours[j]->getTimestep() > timestep){
                    pIsActive = true;
                }

            }
            if (!pIsActive){
                removePassengerFromActive(i);
            }
        }
        for(int i = 0; i < numFutureEvents; i++){
            if (events[i]->timestep == timestep){
                string event_str;
                if (events[i]->isElevatorSpecific){
                    event_str = "on elevator " + to_string(events[i]->id);
                }
                else{
                    event_str = "";
                }
                qDebug() << "Deal with" << events[i]->type.c_str() << event_str.c_str();
                //eventually, have a function that deals with this and takes it off of future........................
            }
        }


        for(int i = 0; i < numElevators; i++){
            int floor = elevators[i]->currFloor;
            if(!elevators[i]->requests.empty()){ //if there are requests...
                if(elevators[i]->requests.find(floor) != elevators[i]->requests.end()){//if the elevator is supposed to stop here...

                    //need to differentiate between current direction and goal direction...

                   qDebug() << "Elevator" << elevators[i]->id << "stopped at floor" << floor << "\n\t Ring bell\n\t Open doors for 10 seconds\n\t Ring bell\n\t Close doors";
                   //add something useful here.........

                   elevators[i]->currState = "idle";
                   elevators[i]->serviceRequest(floor);
                   onboardElevator(elevators[i]);
                }
                else if(elevators[i]->direction == "none"){
                    int nextRequest = elevators[i]->closestRequest();
                    if(nextRequest > floor){
                        elevators[i]->direction = "up";
                        elevators[i]->currState = "moving";
                        qDebug() << "Elevator" << elevators[i]->id << "going up now," << floor << "->" << nextRequest;
                        elevators[i]->currFloor++;//move elevator up one floor..........
                    }
                    else if(nextRequest < floor){
                        elevators[i]->direction = "down";
                        elevators[i]->currState = "moving";
                        qDebug() << "Elevator" << elevators[i]->id << "going down now," << floor << "->" << nextRequest;
                        elevators[i]->currFloor--;//move elevator down one floor..........
                    }
                }
                else if(elevators[i]->direction == "up"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }
                    qDebug() << "Elevator" << elevators[i]->id << "going up," << elevators[i]->currFloor << "->" << elevators[i]->currFloor+1;
                    elevators[i]->currFloor++;//move elevator up one floor..........
                }
                else if(elevators[i]->direction == "down"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }
                    qDebug() << "Elevator" << elevators[i]->id << "going down," << elevators[i]->currFloor << "->" << elevators[i]->currFloor-1;
                    elevators[i]->currFloor--;//move elevator down one floor..........
                }
            }
        }

        timestep++;
    }
    //loop through passengers -> loop through behvaiours-> any behaviours at this timestep? any initial button presses at this timestep?
    //loop through safety events -> any events at this timestep?
    //loop through elevators -> should I stop at this floor? open doors? etc? should I start moving?
}

void SimulationController::onboardElevator(Elevator* elevator){
    for(int k = 0; k < numActivePassengers; k++){
        int i = activePassengers[k];
        qDebug() << "Passenger" << passengers[i]->id << ": isInElevator" << passengers[i]->isInElevator << ", starting floor" << passengers[i]->startingFloor << ", elevator's floor" << elevator->currFloor << ", elevator's direction" << elevator->direction.c_str();
        if(!passengers[i]->isInElevator && passengers[i]->startingFloor==elevator->currFloor && passengers[i]->direction==elevator->direction){
            passengers[i]->boardElevator(elevator);
            qDebug() << "Passenger" << passengers[i]->id << "getting onto" << elevator->id;
        }
    }
}
