#include "safetyevent.h"

SafetyEvent::SafetyEvent(string event, int time, bool singleElevtor, int elevator){
    type = event;
    timestep = time;
    isElevatorSpecific = singleElevtor;
    id = elevator;
}
