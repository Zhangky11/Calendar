#include "signup.h"
#include "ui_signup.h"
#include "dialog.h"
#include "class.h"
extern string get_hash(string);
Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::back_to_login(void)
{
    Dialog *udialog = new Dialog;
    udialog->show();
    this->hide();
}

void Signup::add_user(void)
{
    string user = (ui->lineEdit->text()).toStdString();
    QString pwd = ui->lineEdit_2->text();
    QString pwd_confirm = ui->lineEdit_3->text();
    string fileData;
    string pw_in;
    //检查该账号是否已注册
    ifstream in("users.data");

    if (in)
    {
        while(getline(in,fileData))
        {
            if(!fileData.compare(user))
            {
                ui->lineEdit->clear();
                QMessageBox msgBox;
                msgBox.setText("The user name is already used. Please choose another one.");
                msgBox.exec();
                break;
            }
        }
        in.close();
        return;
    }

    if (pwd==pwd_confirm && pwd.length()==6) {}
    else
    {
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        QMessageBox msgBox;
        msgBox.setText("Wrong password. Failed to create a new account.\n");
        msgBox.exec();
        return;
    }

    pw_in = get_hash(pwd.toStdString());

    //新用户信息录入user文件
    ofstream out("users.data",ofstream::app);
    if (!out)
    {   QMessageBox msgBox;
        msgBox.setText("Error! Unable to save to the file!");
        msgBox.exec();
        return;
    }
    out<<user<<endl;
    out<<pw_in<<endl;
    out.close();

    //用于存放任务
    string data_name = user + ".data";
    ofstream outfile(data_name);
    if (!outfile) {
        QMessageBox msgBox;
        msgBox.setText("Error! Unable to save to the file!");
        msgBox.exec();
        return;
    }
    outfile.close();
    QMessageBox msgBox;
    msgBox.setText("Successfully create user!");
    msgBox.exec();
    back_to_login();
}
