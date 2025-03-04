#ifndef FLOOR_H
#define FLOOR_H
#include <QObject>
#include <string>
using namespace std;

class Floor:public QObject{

    Q_OBJECT
    public:
        Floor(int);
    private:
        int floorNumber;
    //public slots:
        //void handlePress(int, string); //don't know how to match up specific passenger with specific floor...Maybe use straight up public methods instead of this nonesense...
};

#endif // FLOOR_H
