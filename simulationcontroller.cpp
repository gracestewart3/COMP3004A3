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

    //notes: Should probably split into smaller functions?
    int timestep = 0;
    string onGoingSafetyEvents[MAX_ARR];
    int numOnGoingSafetyEvents = 0;
    while (true){ //change to stop  when all events are dealt  with
        string log = "\nTimestep " + to_string(timestep) + ":";
        qDebug() << timestep;
        string tempSafetyEvents[MAX_ARR];
        int numTempSafetyEvents = 0;
        for(int i = 0; i < numElevators; i++){
            int floor = elevators[i]->currFloor;
            if(!elevators[i]->requests.empty()){ //if there are requests...
                if(elevators[i]->requests.find(floor) != elevators[i]->requests.end()){//if the elevator is supposed to stop here...
                   qDebug() << "Elevator" << elevators[i]->id << "stopped at floor" << floor << ":\n\t Ring bell\n\t Open doors for 10 seconds\n\t Ring bell\n\t Close doors";
                   log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " arrives at f" + to_string(floor) + ", bell rings, doors open.....";
                   elevators[i]->door->openDoor();
                   elevators[i]->currState = "idle";
                   elevators[i]->serviceRequest(floor);
                   string subLog1;
                   unloadElevator(elevators[i], &subLog1);
                   log += subLog1;
                   string subLog2;
                   onboardElevator(elevators[i], &subLog2);
                   log += subLog2;
                }
                else if(elevators[i]->direction == "none"){
                    int nextRequest = elevators[i]->closestRequest();
                    if(nextRequest > floor){
                        elevators[i]->direction = "up";
                        elevators[i]->currState = "moving";
                        qDebug() << "Elevator" << elevators[i]->id << "going up now," << floor << "->" << nextRequest;
                        log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(floor) + "->f" + to_string(floor + 1);
                        elevators[i]->currFloor++;//move elevator up one floor..........
                    }
                    else if(nextRequest < floor){
                        elevators[i]->direction = "down";
                        elevators[i]->currState = "moving";
                        qDebug() << "Elevator" << elevators[i]->id << "going down now," << floor << "->" << nextRequest;
                        log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(floor) + "->f" + to_string(floor - 1);
                        elevators[i]->currFloor--;//move elevator down one floor..........
                    }
                }
                else if(elevators[i]->direction == "up"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }
                    qDebug() << "Elevator" << elevators[i]->id << "going up," << elevators[i]->currFloor << "->" << elevators[i]->currFloor+1;
                    log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(elevators[i]->currFloor) + "->f" + to_string(elevators[i]->currFloor + 1);
                    elevators[i]->currFloor++;//move elevator up one floor..........
                }
                else if(elevators[i]->direction == "down"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }
                    qDebug() << "Elevator" << elevators[i]->id << "going down," << elevators[i]->currFloor << "->" << elevators[i]->currFloor-1;
                    log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(elevators[i]->currFloor) + "->f" + to_string(elevators[i]->currFloor - 1);
                    elevators[i]->currFloor--;//move elevator down one floor..........
                }
            }
            else{
                //if no more requests, stop moving
                elevators[i]->goalDirection="none";
                elevators[i]->direction="none";
                elevators[i]->currState="idle";
            }
        }

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
                    string subLog = "";
                    if(btn == "help"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the help button";
                         respondToHelpSignal(&subLog, passengers[i]->inElevator->id, passengers[i]->id);
                         tempSafetyEvents[numTempSafetyEvents] = "Help Request on e" + to_string(passengers[i]->inElevator->id);

                    }
                    else if(btn == "close door"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the close door button";
                         //eventually do something useful here........................................
                    }
                    else if(btn == "open door"){
                         qDebug() << "Passenger" << passengers[i]->id << "pressed the open door button";
                         //eventually do something useful here........................................
                    }

                    log += subLog;

                }
                else if (passengers[i]->behaviours[j]->getTimestep() > timestep){
                    pIsActive = true;
                }

            }
            if (!pIsActive){

                removePassengerFromActive(i);
            }
        }
        for(int k = 0; k < numFutureEvents; k++){
            int i = futureEvents[k];
            if (events[i]->timestep == timestep){
                if(events[i]->type=="Fire Alarm (from building)"){
                    string subLog="";
                    respondToBuildingFireAlarm(&subLog);
                    log +=subLog;
                    onGoingSafetyEvents[numOnGoingSafetyEvents] = "Fire Alarm (from building)";
                    numOnGoingSafetyEvents++;
                }
                else if(events[i]->type=="Power Outage"){
                    string subLog="";
                    respondToPowerOutage(&subLog);
                    log +=subLog;
                    onGoingSafetyEvents[numOnGoingSafetyEvents] = "Power Outage";
                    numOnGoingSafetyEvents++;
                }
                else if(events[i]->type=="Fire Alarm (from elevator)"){
                    string subLog="";
                    respondToElevatorFireAlarm(&subLog, events[i]->id);
                    log +=subLog;
                    onGoingSafetyEvents[numOnGoingSafetyEvents] = "Fire Alarm (from e" + to_string(events[i]->id) + ")";
                    numOnGoingSafetyEvents++;
                }

                else if(events[i]->type=="Overload"){
                    string subLog="";
                    respondToOverload(&subLog, events[i]->id);
                    log +=subLog;
                    tempSafetyEvents[numTempSafetyEvents] = "Overload (on e" + to_string(events[i]->id) + ")";
                    numTempSafetyEvents++;
                }
                else if(events[i]->type=="Door Obstruction"){
                    string subLog="";
                    respondToDoorObstruction(&subLog, events[i]->id);
                    log +=subLog;
                    tempSafetyEvents[numTempSafetyEvents] = "Door obstructed (on e" + to_string(events[i]->id) + ")";
                    numTempSafetyEvents++;
                }
                removeEventFromFuture(i);


            }
        }

        //loop through elevators and close any open doors
        for(int i = 0; i < numElevators; i++){
            if(elevators[i]->door->doorOpen){
                elevators[i]->door->closeDoor();
                log += "\n\tElevator Event:.....bell rings, doors close on e" +  to_string(elevators[i]->id);
            }
        }

        sleep(1);
        string currSafetyEvents = "";
        for(int i=0; i<numOnGoingSafetyEvents; i++){
            currSafetyEvents +=  onGoingSafetyEvents[i] + "\n";
        }
        for(int i=0; i<numTempSafetyEvents; i++){
            currSafetyEvents += tempSafetyEvents[i] + "\n";
        }
        emit updateTimestep(timestep);
        emit updateLog(QString::fromStdString(log));
        emit updateSafetyEvents(QString::fromStdString(currSafetyEvents));


        bool elevatorsAllFinished = true;
        for(int i = 0; i < numElevators; i++){
            if(elevators[i]->currState!="idle" || elevators[i]->goalDirection!="none" || elevators[i]->direction!="none"){
                elevatorsAllFinished = false;
            }
        }

        if(numFutureEvents==0 && numActivePassengers==0 && elevatorsAllFinished){
           string finishedMessage = "\tAll events handled";
           emit updateLog(QString::fromStdString(finishedMessage));
           break;
        }
        timestep++;

    }
}

void SimulationController::onboardElevator(Elevator* elevator, string* log){

    for(int k = 0; k < numActivePassengers; k++){
        int i = activePassengers[k];
        if(!passengers[i]->isInElevator && passengers[i]->startingFloor==elevator->currFloor && passengers[i]->direction==elevator->goalDirection){
            passengers[i]->boardElevator(elevator);
            *log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " enters e" + to_string(elevator->id);
            qDebug() << "Passenger" << passengers[i]->id << "getting onto" << elevator->id;
            passengers[i]->boardElevator(elevator);
            *log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " requests f"  + to_string(passengers[i]->destination) + " on e" + to_string(elevator->id);
            qDebug() << "Passenger" << passengers[i]->id << "requesting floor" << passengers[i]->destination;
            passengers[i]->pushDestinationButton();
        }
    }
    elevator->direction = elevator->goalDirection;
}

void SimulationController::unloadElevator(Elevator* elevator, string* log){
    if(elevator->recalled  && elevator->recalledFloor == elevator->currFloor){
        *log += "\n\tElevator Event: e" + to_string(elevator->id) + " asks passengers to disembark via audio/visual displays.";
    }
    for(int i = 0; i < numPassengers; i++){
        if((passengers[i]->isInElevator && passengers[i]->inElevator==elevator && passengers[i]->destination==elevator->currFloor) || (passengers[i]->isInElevator && passengers[i]->inElevator==elevator  && elevator->recalled  && elevator->recalledFloor == elevator->currFloor)){
            *log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " disembarks e" + to_string(elevator->id) + " on f" + to_string(elevator->currFloor);
            passengers[i]->disembarkElevator();
        }
    }
}

void  SimulationController::respondToBuildingFireAlarm(string*  log){
    *log += "\n\tSafety Event: Building fire alarm. Recalling all elevators to f1.";
    for(int i = 0; i < numElevators; i++){
        elevators[i]->recallToFloor(1);
    }
    //say that all passengers are inactive and all events are passed (bit of a cheat, reconsider later))
    numActivePassengers=0;
    delete [] activePassengers;

    //numFutureEvents=0;
    //delete [] futureEvents;
}

void  SimulationController::respondToPowerOutage(string*  log){
    *log += "\n\tSafety Event: Building power outage. Recalling all elevators to f1 on emergency power.";
    for(int i = 0; i < numElevators; i++){
        elevators[i]->recallToFloor(1);
    }
    //say that all passengers are inactive and all events are passed (bit of a cheat, reconsider later))
    numActivePassengers=0;
    delete [] activePassengers;

    numFutureEvents=0;
    delete [] futureEvents;
}

void  SimulationController::respondToElevatorFireAlarm(string*  log, int el){
    *log += "\n\tSafety Event: A fire alarm from e" +  to_string(el) +". Recalling e" +  to_string(el) + " to f1.";
    for(int i = 0; i < numElevators; i++){
        if(elevators[i]->id==el){
            elevators[i]->recallToFloor(1);
        }
    }
    //say that all passengers in that elevator are inactive (bit of a cheat, reconsider later)
    for(int k = 0; k < numActivePassengers; k++){
        int i = activePassengers[k];
        if(passengers[i]->isInElevator && passengers[i]->inElevator->id == el){
            removePassengerFromActive(i);
        }
    }
}

void  SimulationController::respondToOverload(string*  log, int el){
    *log += "\n\tSafety Event: Overload on e" +  to_string(el) +". Asking passengers to decrease load via audio/visual displays.";
}

void  SimulationController::respondToDoorObstruction(string*  log, int el){
    *log += "\n\tSafety Event: Doors obstructed on e" +  to_string(el) +". Opening doors.";
    for(int i = 0; i < numElevators; i++){
        if(elevators[i]->id==el){
            elevators[i]->door->openDoor();
        }
    }
}

void  SimulationController::respondToHelpSignal(string*  log, int el, int psg){
    *log += "\n\tSafety Event: p"  + to_string(psg) + " requested help on e" + to_string(el) +". Calling building safety services.";
}

void  SimulationController::respondToDoorOpen(string*  log, int el, int psg){
    *log += "\n\tPassenger Event: p"  + to_string(psg) + " pressed 'Doors Open' on e" + to_string(el) +". Opening doors.";
    for(int i = 0; i < numElevators; i++){
        if(elevators[i]->id==el){
            elevators[i]->door->openDoor();
        }
    }
}

