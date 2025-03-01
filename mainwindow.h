#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addeventdialog.h"
#include "addpassengerdialog.h"
#include "passenger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_event_clicked();
    void on_add_passenger_clicked();

private:
    Ui::MainWindow *ui;
    AddEventDialog *eventWindow;
    AddPassengerDialog *passengerWindow;

public slots:
    void handleNewPassenger(int, int, Behaviour**, string);
};
#endif // MAINWINDOW_H
