#include "dialog.h"
#include "ui_dialog.h"
#include "class.h"
extern string get_hash(string);
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::login_in()
{
    string user = (ui->lineEdit->text()).toStdString();
    QString pwd = ui->lineEdit_2->text();
    string pw_in;
    string fileData;
    bool flag_1 = false;
    pw_in = get_hash(pwd.toStdString());
    if (user == "root" && pw_in == "e10adc3949ba59abbe56e057f20f883e") {flag_1 = true;} //user中无超级用户信息，故不从中查找
    else
    {
        ifstream in("users.data");
        while(getline(in,fileData))
        {
            if(!fileData.compare(user))
            {
                getline(in, fileData);
                if(!fileData.compare(pw_in))
                {
                    flag_1 = true;
                    break;
                }
            }
        }
        in.close();

        if (flag_1)
        {
            this->hide();
            User *ucal = new User(user);
            ucal->show();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("The user name or password is wrong! Please enter them again.");
            msgBox.exec();
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
        }
    }
}

void Dialog::sign_up()
{
    this->hide();
    Signup *usign = new Signup;
    usign->show();
}

void Dialog::quit()
{
    this->close();
}
