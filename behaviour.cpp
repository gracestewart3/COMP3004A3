#include "behaviour.h"

Behaviour::Behaviour(int time, string btn){
    timestep = time;
    buttonPressed = btn;
}

int Behaviour::getTimestep(){
    return timestep;
}

string Behaviour::getButton(){
    return buttonPressed;
}
