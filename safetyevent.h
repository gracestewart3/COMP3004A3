#ifndef SAFETYEVENT_H
#define SAFETYEVENT_H
#include <string>
using namespace std;

class SafetyEvent{
public:
    SafetyEvent(string, int, bool=false, int=-1);
    string type;
    int timestep;
    bool isElevatorSpecific;
    int id;

};

#endif // SAFETYEVENT_H
