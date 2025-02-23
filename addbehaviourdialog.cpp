#include "addbehaviourdialog.h"
#include "ui_addbehaviourdialog.h"
#include <QPushButton>

AddBehaviourDialog::AddBehaviourDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBehaviourDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Add");
}

AddBehaviourDialog::~AddBehaviourDialog()
{
    delete ui;
}
