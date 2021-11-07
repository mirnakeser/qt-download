#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    setWindowTitle("Dialog");

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &Dialog::dohvati);
}

void Dialog::dohvati()
{
    username = ui->usernameLineEdit->text();
    password = ui->passwordLineEdit->text();
}

Dialog::~Dialog()
{
    delete ui;
}
