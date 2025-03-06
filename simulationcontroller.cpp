#include "simulationcontroller.h"

SimulationController::SimulationController(SafetyEvent** evs, int numEvs, Passenger** pgs, int numPgs, int numEls, int numFls){
    numFloors = numFls;
    numElevators = numEls;
    isPaused = false;
    isStopped = false;

    elevators = new Elevator*[numElevators];
    for(int i = 0; i < numElevators; i++){
        elevators[i] = new Elevator(i);
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
    //remove the given index from the array of indexes belonging to "active" passengers
    int newSize = numActivePassengers-1;
    if (newSize <= 0){
        activePassengers = nullptr;
        numActivePassengers = 0;
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
    activePassengers = temp;
    numActivePassengers--;

}

void SimulationController::removeEventFromFuture(int index){
    //remove the given index from the array of indexes belonging to future events
    int newSize = numFutureEvents-1;
    if (newSize <= 0){
        delete [] futureEvents;
        futureEvents = nullptr;
        numFutureEvents = 0;
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

}

int SimulationController::selectElevator(int flr, string goalDir){
    //select which elevator to assign  to a floor request baased on direction and proximity
    int best = 0;
    for(int i = 0; i < numElevators; i++){
        int oldDistance = abs(elevators[best]->currFloor - flr);
        int newDistance = abs(elevators[i]->currFloor - flr);

        bool newRightDirection = (elevators[i]->goalDirection == goalDir);

        if(((newDistance <= oldDistance) && newRightDirection) || ((newDistance <= oldDistance) && elevators[i]->goalDirection=="none")){
            best = i;
        }
    }
    return best;
}

void SimulationController::informElevatorOfRequest(int el, int flr, string goalDir){
    //inform elevator of a new floor request
    elevators[el]->enqueueRequest(flr);
    elevators[el]->goalDirection = goalDir;
}


void SimulationController::runSimulation(){
    //run the simulation

    int timestep = 0;
    string onGoingSafetyEvents[MAX_ARR];
    int numOnGoingSafetyEvents = 0;

    while (true){
        if(isStopped){
            emit updateLog(QString::fromStdString("\nSIMULATION STOPPED"));
            emit endSimulation();
            return;
        }
        if(isPaused){
            sleep(1);
            continue;
        }

        string log = "\nTimestep " + to_string(timestep) + ":";
        string tempSafetyEvents[MAX_ARR];
        int numTempSafetyEvents = 0;

        //move elevators and/or see if they should stop
        for(int i = 0; i < numElevators; i++){
            int floor = elevators[i]->currFloor;
            if(!elevators[i]->requests.empty()){
                if(elevators[i]->requests.find(floor) != elevators[i]->requests.end()){
                   log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " arrives at f" + to_string(floor) + ", bell rings and doors open";
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
                        log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(floor) + "->f" + to_string(floor + 1);
                        elevators[i]->currFloor++;
                    }
                    else if(nextRequest < floor){
                        elevators[i]->direction = "down";
                        elevators[i]->currState = "moving";
                        log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(floor) + "->f" + to_string(floor - 1);
                        elevators[i]->currFloor--;
                    }
                }
                else if(elevators[i]->direction == "up"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }

                    log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(elevators[i]->currFloor) + "->f" + to_string(elevators[i]->currFloor + 1);
                    elevators[i]->currFloor++;
                }
                else if(elevators[i]->direction == "down"){
                    if(elevators[i]->currState == "idle"){
                        elevators[i]->currState = "moving";
                    }

                    log += "\n\tElevator Event: e" + to_string(elevators[i]->id) + " f" + to_string(elevators[i]->currFloor) + "->f" + to_string(elevators[i]->currFloor - 1);
                    elevators[i]->currFloor--;
                }
            }
            else{
                //if no more requests, stop moving
                elevators[i]->goalDirection="none";
                elevators[i]->direction="none";
                elevators[i]->currState="idle";
            }
        }

        //looop through passenngers to check for floor requests or other behaviours
        for(int k = 0; k < numActivePassengers; k++){
            int i = activePassengers[k];
            bool pIsActive = false;
            if (passengers[i]->floorRequestTimeStep == timestep){
                int startingFloor = passengers[i]->startingFloor;
                if((startingFloor <= 1 && passengers[i]->direction=="down") || (startingFloor >= numFloors && passengers[i]->direction=="up") || (startingFloor <1)  || (startingFloor>numFloors)){
                    log += "\n\tError: p" + to_string(passengers[i]->id)  +  " requested " + passengers[i]->direction + " on f" + to_string(startingFloor) + ". Disallowed by system.";
                    removePassengerFromActive(i);
                }
                else{
                    int assignedElevator = selectElevator(startingFloor, passengers[i]->direction);
                    informElevatorOfRequest(assignedElevator, startingFloor, passengers[i]->direction);

                    log += "\n\tFloor Request: p" + to_string(passengers[i]->id)  +  " f" + to_string(startingFloor) + " " + passengers[i]->direction;
                }

            }
            if (passengers[i]->floorRequestTimeStep > timestep || !passengers[i]->isInElevator){
                pIsActive = true;
            }
            for(int j = 0; j < passengers[i]->numBehaviours; j++){

                if (passengers[i]->behaviours[j]->getTimestep() == timestep && passengers[i]->isInElevator){
                    string btn = passengers[i]->behaviours[j]->getButton();
                    string subLog = "";
                    if(btn == "help"){
                         respondToHelpSignal(&subLog, passengers[i]->inElevator->id, passengers[i]->id);
                         tempSafetyEvents[numTempSafetyEvents] = "Help Request on e" + to_string(passengers[i]->inElevator->id);

                    }
                    else if(btn == "close door"){
                         respondToDoorClose(&subLog, passengers[i]->inElevator->id, passengers[i]->id);
                    }
                    else if(btn == "open door"){
                         respondToDoorOpen(&subLog, passengers[i]->inElevator->id, passengers[i]->id);
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

        //loop through events and check for any occuring at the current timestep
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
                log += "\n\tElevator Event: Bell rings and doors close on e" +  to_string(elevators[i]->id);
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

        //update GUI
        emit updateTimestep(timestep);
        emit updateLog(QString::fromStdString(log));
        emit updateSafetyEvents(QString::fromStdString(currSafetyEvents));
        emit updateElevators(QString::fromStdString(getElevatorStr()));


        bool elevatorsAllFinished = true;
        for(int i = 0; i < numElevators; i++){
            if(elevators[i]->currState!="idle" || elevators[i]->goalDirection!="none" || elevators[i]->direction!="none"  || elevators[i]->recalled){
                elevatorsAllFinished = false;
            }
        }

        //check if simulation should end
        if(numFutureEvents==0 && numActivePassengers==0 && elevatorsAllFinished){
           string finishedMessage = "\tAll events handled";
           emit updateLog(QString::fromStdString(finishedMessage));
           emit endSimulation();
           return;
        }
        timestep++;

    }

}

void SimulationController::onboardElevator(Elevator* elevator, string* log){
    //board passengers onto the given elevator at the given floor
    for(int k = 0; k < numActivePassengers; k++){
        int i = activePassengers[k];
        if(!passengers[i]->isInElevator && passengers[i]->startingFloor==elevator->currFloor && passengers[i]->direction==elevator->goalDirection){
            passengers[i]->boardElevator(elevator);
            *log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " enters e" + to_string(elevator->id);
            passengers[i]->boardElevator(elevator);
            *log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " requests f"  + to_string(passengers[i]->destination) + " on e" + to_string(elevator->id);
            passengers[i]->pushDestinationButton();
        }
    }
    elevator->direction = elevator->goalDirection;
}

void SimulationController::unloadElevator(Elevator* elevator, string* log){
    //unload passengers from the given elevator onto the  given floor
    if(elevator->recalled  && elevator->recalledFloor == elevator->currFloor){
        *log += "\n\tElevator Event: e" + to_string(elevator->id) + " asks passengers to disembark via audio/visual displays.";

    }
    for(int i = 0; i < numPassengers; i++){
        if((passengers[i]->isInElevator && passengers[i]->inElevator==elevator && passengers[i]->destination==elevator->currFloor) || (passengers[i]->isInElevator && passengers[i]->inElevator==elevator  && elevator->recalled  && elevator->recalledFloor == elevator->currFloor)){
            *log += "\n\tPassenger Event: p" + to_string(passengers[i]->id) + " disembarks e" + to_string(elevator->id) + " on f" + to_string(elevator->currFloor);
            passengers[i]->disembarkElevator();
        }
    }
    elevator->recalled = false;
}

void  SimulationController::respondToBuildingFireAlarm(string*  log){
    *log += "\n\tSafety Event: Building fire alarm. Recalling all elevators to f1.";
    for(int i = 0; i < numElevators; i++){
        elevators[i]->recallToFloor(1);
    }
    //say that all passengers are inactive and all events are passed (bit of a cheat, reconsider later))
    numActivePassengers=0;
    delete [] activePassengers;

}

void  SimulationController::respondToPowerOutage(string*  log){
    *log += "\n\tSafety Event: Building power outage. Recalling all elevators to f1 on emergency power.";
    for(int i = 0; i < numElevators; i++){
        elevators[i]->recallToFloor(1);
    }
    //say that all passengers are inactive and all events are passed (bit of a cheat, reconsider later))
    numActivePassengers=0;
    delete [] activePassengers;
}

void  SimulationController::respondToElevatorFireAlarm(string*  log, int el){
    *log += "\n\tSafety Event: A fire alarm from e" +  to_string(el) +". Recalling e" +  to_string(el) + " to f1.";
    for(int i = 0; i < numElevators; i++){
        if(elevators[i]->id==el){
            elevators[i]->recallToFloor(1);
        }
    }

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
    for(int i = 0; i < numElevators; i++){
        if(elevators[i]->id==el){
            if(elevators[i]->currState=="idle"){
                elevators[i]->door->openDoor();
                *log += "\n\tPassenger Event: p"  + to_string(psg) + " pressed 'Open Doors' on e" + to_string(el) +". Opening doors beyond default period.";
            }
            else{
               *log += "\n\tPassenger Event: p"  + to_string(psg) + " pressed 'Open Doors' on e" + to_string(el) + ". Elevator is moving, so doors stay closed.";
            }
        }
    }
}

void  SimulationController::respondToDoorClose(string*  log, int el, int psg){

    for(int i = 0; i < numElevators; i++){
        if(elevators[i]->id==el){
            if(elevators[i]->door->doorOpen){
                elevators[i]->door->closeDoor();
                *log += "\n\tPassenger Event: p"  + to_string(psg) + " pressed 'Close Doors' on e" + to_string(el) +". Closing doors early.";
            }
            else{
                *log += "\n\tPassenger Event: p"  + to_string(psg) + " pressed 'Close Doors' on e" + to_string(el) +". Doors are already closed, so nothing happens.";
            }
        }
    }
}

void SimulationController::pause(){
    isPaused = true;
}

void SimulationController::resume(){
    isPaused = false;
}

void SimulationController::stop(){
    isStopped = true;
}

string SimulationController::getElevatorStr(){
    string txt = "";
    for(int i = 0; i < numElevators; i++){
        txt += "Elevator e" + to_string(elevators[i]->id) + ": on floor " + to_string(elevators[i]->currFloor) + ", currently " + elevators[i]->currState + "\n";
    }
    return txt;
}
