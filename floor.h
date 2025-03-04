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
};

#endif // FLOOR_H
