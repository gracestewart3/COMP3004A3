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

int SimulationController::selectElevator(int flr, string goalDir){
    int best = 0;
    for(int i = 0; i < numElevators; i++){
        int oldDistance = abs(elevators[best]->currFloor - flr);
        int newDistance = abs(elevators[i]->currFloor - flr);

        //bool newRightDirection = (elevators[i]->currFloor >= flr && elevators[i]->direction=="down") || (elevators[i]->currFloor <= flr and elevators[i]->direction=="up");
        bool newRightDirection = (elevators[i]->goalDirection == goalDir);

        if(((newDistance <= oldDistance) && newRightDirection) || ((newDistance <= oldDistance) && elevators[i]->goalDirection=="none")){
            best = i;
        }
    }
    return best;
}

void SimulationController::informElevatorOfRequest(int el, int flr, string goalDir){
    elevators[el]->enqueueRequest(flr);
    elevators[el]->goalDirection = goalDir; //this seems like it could be problematic
}


void SimulationController::runSimulation(){

    //notes: Should probably split into smaller functions. Still need to add support for car request, and how to keep track of what passengers are where
    int timestep = 0;
    while (timestep  <= 10){ //change to stop  when all events are dealt  with
        string log = "Timestep " + to_string(timestep) + ":";
        qDebug() << timestep;
        for(int k = 0; k < numActivePassengers; k++){
            int i = activePassengers[k];
            bool pIsActive = false;
            if (passengers[i]->floorRequestTimeStep == timestep){
                //note: right now I am bypassing floors all together. Mention this in assignment
                int startingFloor = passengers[i]->startingFloor;
                int assignedElevator = selectElevator(startingFloor, passengers[i]->direction);
                informElevatorOfRequest(assignedElevator, startingFloor, passengers[i]->direction);
                qDebug() << "Passenger" << passengers[i]->id << "selects" << passengers[i]->direction.c_str() << "on floor" << startingFloor << ".";

                log += "\n\tFloor Request: p" + to_string(passengers[i]->id)  +  " f" + to_string(startingFloor) + " " + passengers[i]->direction;

            }
            if (passengers[i]->floorRequestTimeStep > timestep || !passengers[i]->isInElevator){
                pIsActive = true;
            }
            for(int j = 0; j < passengers[i]->numBehaviours; j++){

                if (passengers[i]->behaviours[j]->getTimestep() == timestep && passengers[i]->isInElevator){
                    string btn = passengers[i]->behaviours[j]->getButton();

                    //why is it crashing on a help event??

                    if(btn == "help"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the help button";
                         //eventually do something useful here........................................

                    }
                    else if(btn == "close door"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the close door button";
                         //eventually do something useful here........................................
                    }
                    else if(btn == "open door"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the open door button";
                         //eventually do something useful here........................................
                    }

                    log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " e" + to_string(passengers[i]->inElevator->id)+ " " + btn;

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
                    event_str = " e" + to_string(events[i]->id);
                }
                else{
                    event_str = "";
                }
                qDebug() << "Deal with" << events[i]->type.c_str() << event_str.c_str();
                log += "\n\tSafety Event: " + events[i]->type + event_str;
                //eventually, have a function that deals with this and takes it off of future........................
            }
        }


        for(int i = 0; i < numElevators; i++){
            int floor = elevators[i]->currFloor;
            if(!elevators[i]->requests.empty()){ //if there are requests...
                if(elevators[i]->requests.find(floor) != elevators[i]->requests.end()){//if the elevator is supposed to stop here...
                   qDebug() << "Elevator" << elevators[i]->id << "stopped at floor" << floor << ":\n\t Ring bell\n\t Open doors for 10 seconds\n\t Ring bell\n\t Close doors";
                   log += "\n\tElevator: e" + to_string(elevators[i]->id) + " arrives at f" + to_string(floor) + ", bell rings, doors open";
                   //add something useful here.........

                   elevators[i]->currState = "idle";
                   elevators[i]->serviceRequest(floor);
                   onboardElevator(elevators[i]);
                   log += ", bell rings, doors close";
                }
                else if(elevators[i]->direction == "none"){
                    int nextRequest = elevators[i]->closestRequest();
                    if(nextRequest > floor){
                        elevators[i]->direction = "up";
                        elevators[i]->currState = "moving";
                        qDebug() << "Elevator" << elevators[i]->id << "going up now," << floor << "->" << nextRequest;
                        log += "\n\tElevator: e" + to_string(elevators[i]->id) + " f" + to_string(floor) + "->f" + to_string(floor + 1);
                        elevators[i]->currFloor++;//move elevator up one floor..........
                    }
                    else if(nextRequest < floor){
                        elevators[i]->direction = "down";
                        elevators[i]->currState = "moving";
                        qDebug() << "Elevator" << elevators[i]->id << "going down now," << floor << "->" << nextRequest;
                        log += "\n\tElevator: e" + to_string(elevators[i]->id) + " f" + to_string(floor) + "->f" + to_string(floor - 1);
                        elevators[i]->currFloor--;//move elevator down one floor..........
                    }
                }
                else if(elevators[i]->direction == "up"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }
                    qDebug() << "Elevator" << elevators[i]->id << "going up," << elevators[i]->currFloor << "->" << elevators[i]->currFloor+1;
                    log += "\n\tElevator: e" + to_string(elevators[i]->id) + " f" + to_string(elevators[i]->currFloor) + "->f" + to_string(elevators[i]->currFloor + 1);
                    elevators[i]->currFloor++;//move elevator up one floor..........
                }
                else if(elevators[i]->direction == "down"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }
                    qDebug() << "Elevator" << elevators[i]->id << "going down," << elevators[i]->currFloor << "->" << elevators[i]->currFloor-1;
                    log += "\n\tElevator: e" + to_string(elevators[i]->id) + " f" + to_string(elevators[i]->currFloor) + "->f" + to_string(elevators[i]->currFloor - 1);
                    elevators[i]->currFloor--;//move elevator down one floor..........
                }
            }
            else{
                //if no more requestss, stop moving. (not sure about this)
                elevators[i]->goalDirection="none";
                elevators[i]->direction="none";
            }
        }
        log += "\n";
        sleep(1);
        timestep++;
        emit updateTimestep(timestep);
        emit updateLog(QString::fromStdString(log));
    }
}

void SimulationController::onboardElevator(Elevator* elevator){
    for(int k = 0; k < numActivePassengers; k++){
        int i = activePassengers[k];
        if(!passengers[i]->isInElevator && passengers[i]->startingFloor==elevator->currFloor && passengers[i]->direction==elevator->goalDirection){
            passengers[i]->boardElevator(elevator);
            qDebug() << "Passenger" << passengers[i]->id << "getting onto" << elevator->id;
            passengers[i]->boardElevator(elevator);
            qDebug() << "Passenger" << passengers[i]->id << "requesting floor" << passengers[i]->destination;
            passengers[i]->pushDestinationButton();
        }
    }
    elevator->direction = elevator->goalDirection;
}
