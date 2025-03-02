#include "simulationcontroller.h"

SimulationController::SimulationController(SafetyEvent** evs, int numEvs, Passenger** pgs, int numPgs, int numEls, int numFls, bool complete){
    simulationIsComplete = complete;
    numElevators = numEls;
    elevators = new Elevator*[numElevators];
    for(int i = 0; i < numElevators; i++){
        elevators[i] = new Elevator(i);
    }
    numFloors = numFls;
    floors = new Floor*[numFloors];
    for(int i = 0; i < numFloors; i++){
        floors[i] = new Floor(i+1);
    }

    events = evs;
    numEvents = numEvs;
    futureEvents = new int[numEvents];
    for(int i = 0; i < numEvents; i++){
        futureEvents[i] = i;
    }

    passengers = pgs;
    numPassengers = numPgs;
    activePassengers = new int[numPassengers];
    for(int i = 0; i < numPassengers; i++){
        activePassengers[i] = passengers[i]->id;
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
