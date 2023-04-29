#include "user.h"
#include "ui_user.h"

User::User(string user, QWidget *parent):
    QWidget(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    string wel_msg;
    wel_msg = user.append(", Welcome to Calendar!");
    ui->label->setText(QString::fromStdString(wel_msg));
}

User::~User()
{
    delete ui;
}
