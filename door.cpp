#include "door.h"

Door::Door(bool isOpen){
    doorOpen = isOpen;
}

void Door::openDoor(){
    doorOpen = true;
}

void Door::closeDoor(){
    doorOpen = false;
}
