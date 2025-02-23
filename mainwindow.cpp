#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_event_clicked()
{
    eventWindow = new AddEventDialog(this);
    eventWindow->show();

}

void MainWindow::on_add_passenger_clicked()
{
    passengerWindow = new AddPassengerDialog(this);
    passengerWindow->show();

}
