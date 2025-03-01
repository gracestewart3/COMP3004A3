#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include <string>
using namespace std;

class Behaviour{
    public:
        Behaviour(int, string);
    private:
        int timestep;
        string buttonPressed;

};

#endif // BEHAVIOUR_H
