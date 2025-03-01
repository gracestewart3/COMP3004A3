#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);

}

AddEventDialog::~AddEventDialog()
{
    delete ui;
}

