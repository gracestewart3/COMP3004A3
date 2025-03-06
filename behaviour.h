/*
 CLASS NAME: Behaviour
 PURPOSE: To represent a passenger behaviour occuring at a specific timestep in the simulation (eg. open doors)
*/
#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include <string>
using namespace std;

class Behaviour{
    public:
        Behaviour(int, string);
        int getTimestep();
        string getButton();
    private:
        int timestep;
        string buttonPressed;

};

#endif // BEHAVIOUR_H
