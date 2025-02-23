#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addeventdialog.h"
#include "addpassengerdialog.h"

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
};
#endif // MAINWINDOW_H
