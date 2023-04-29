#include "user.h"
#include "ui_user.h"
#include "dialog.h"
#include "class.h"
#include "add_task.h"

bool isFileExists_ifstream(string& name)
{
    ifstream f(name.c_str());
    return f.good();
}

int string2int(string str)
{
    stringstream stream;  //声明一个之后所需的流对象
    int n;
    stream<<str;
    stream>>n;
    stream.clear();//同一个流对象两次使用时应该用clear函数清除流标志，否则下一块就不起作用
    return n;
}

User::User(string user, int count, map<key,task> mytask, QWidget *parent):
    QWidget(parent),
    ui(new Ui::User)
{
    // set title
    ui->setupUi(this);
    string wel_msg;
    this->_id = user;
    wel_msg = user;
    wel_msg = wel_msg.append(", Welcome to Calendar!");
    ui->label->setText(QString::fromStdString(wel_msg));
    //表格设置
    QStandardItemModel* m_model;
    m_model = new QStandardItemModel(0,7);
    QStringList labels = QObject::trUtf8("Name,ID,Pri,Type,Start,Remind,Status").simplified().split(",");
    m_model->setHorizontalHeaderLabels(labels);
    ui->table->setModel(m_model);

    string display_mode = (ui->comboBox->currentText()).toStdString();

    load_task_from_file();

    if(display_mode=="StartTime")//按照启动时间顺序展示
    {
        if(mytask.size()==0)
        {
            return;
        }

        for(auto &t : mytask)
        {
            display(t.second, m_model);
        }
    }

//    else if(display_mode=="Priority")//按照优先级展示 优先级内部仍然按照启动时间展示
//    {
//        if(mytask.size()==0)
//        {
//            printf("There are no planned task.\n");
//            return;
//        }

//        cout<<"Tasks with high priority:\n";
//        for(auto &t : mytask)
//        {
//            if(t.second.priority==0)
//            {
//                print(t.second);
//            }
//        }
//        cout<<"Tasks with medium priority:\n";
//        for(auto &t : mytask)
//        {
//            if(t.second.priority==1)
//            {
//                print(t.second);
//            }
//        }
//        cout<<"Tasks with low priority:\n";
//        for(auto &t : mytask)
//        {
//            if(t.second.priority==2)
//            {
//                print(t.second);
//            }
//        }
//        printf("Tasks display complete!\n");
//    }

//    else//按照任务类型展示 类型内部仍然按照启动时间展示
//   {
//       if(mytask.size()==0)
//       {
//           printf("There are no planned task.\n");
//           return;
//       }

//       cout<<"Tasks with study type:\n";
//       for(auto &t : mytask)
//       {
//           if(t.second.type==0)
//           {
//               print(t.second);
//            }
//       }
//       cout<<"Tasks with entertainment type:\n";
//       for(auto &t : mytask)
//       {
//           if(t.second.type==1)
//           {
//               print(t.second);
//           }
//       }
//       cout<<"Tasks with living type:\n";
//       for(auto &t : mytask)
//       {
//           if(t.second.type==2)
//           {
//               print(t.second);
//           }
//       }
//       printf("Tasks display complete!\n");
//   }
    ui->table->verticalHeader()->hide();
}

void User::load_task_from_file()
{
    string data_id = this->_id + ".data";
    if(!isFileExists_ifstream(data_id))
    {
        printf("File error！\n");
        return;
    }
    fstream in;
    in.open(data_id, ios::in);
    if(in.fail())
    {
        cerr<<"error：Failed to read the user file."<<endl;
        assert(0);
    }
    string name_;
    string id_;
    string prio_;
    string type_;
    string starttime1;string starttime2;
    string remindtime1;string remindtime2;
    string flag_;
    int prio;int type;
    key tmp_key;
    task tmp_task;
    while (1)
    {
        in>>id_;

        if(in.eof())
        {
            break;
        }
        in>>name_;
        in>>starttime1;in>>starttime2;
        in>>type_;
        if(type_=="0") type=0;
        else if(type_=="1") type=1;
        else type=2;
        in>>prio_;
        if(prio_=="0") prio=0;
        else if(prio_=="1") prio=1;
        else prio=2;
        in>>remindtime1;in>>remindtime2;
        in>>flag_;
        tmp_key.task_id=id_;
        tmp_task.task_id=tmp_key.task_id;
        tmp_task.name=name_;
        tmp_task.start_time=starttime1.append(" ").append(starttime2);
        tmp_task.type=string2int(type_);
        tmp_task.priority=string2int(prio_);
        tmp_task.remind_time=remindtime1.append(" ").append(remindtime2);
        if(flag_=="Incomplete")
            tmp_task.flag=false;
        else tmp_task.flag=true;
        tmp_task.remind_flag=false;//任务加载时默认该任务没有被提醒过 需要被重新提醒
        this->mytask.insert(make_pair(tmp_key,tmp_task));
    }
    in.close();
    string tmp_id=this->_id;
    string file_count=tmp_id.append("count.data");
    if(!isFileExists_ifstream(file_count))
    {
        return;
    }
    in.open(file_count, ios::in);
    if(in.fail())
    {
        cerr<<"error：Failed to read the count file."<<endl;
        assert(0);
    }
    in>>this->count;
    in.close();
    in.open(file_count, ios::trunc);
    in.close();
}


void User::display(task task, QStandardItemModel* m_model)
{
    QList<QStandardItem*> add_item;
    //    add_items << new QStandardItem(QString::fromStdString("Calendar")) << new QStandardItem(QString::number(1)) \
    //    << new QStandardItem(QString::fromStdString("High")) << new QStandardItem(QString::fromStdString("Entertain")) \
    //    << new QStandardItem(QString::fromStdString("04/29")) << new QStandardItem(QString::fromStdString("04/29")) \
    //    << new QStandardItem(QString::fromStdString("unfinished"));
    //    m_model->appendRow(add_items);

    // print task name
    add_item << new QStandardItem(QString::fromStdString(task.name));

    // print task id
    add_item << new QStandardItem(QString::fromStdString(task.task_id));

    // print priority
    string priority;
    if (task.priority == 0)
        priority = "High";
    else if (task.priority == 1)
        priority = "Medium";
    else
        priority = "Low";
    add_item << new QStandardItem(QString::fromStdString(priority));

    // print type
    string type;
    if (task.type == 0)
        priority = "Study";
    else if (task.type == 1)
        priority = "Entertainment";
    else
        priority = "Living";
    add_item << new QStandardItem(QString::fromStdString(type));

    // print start time
    add_item << new QStandardItem(QString::fromStdString(task.start_time));

    // print remind time
    add_item << new QStandardItem(QString::fromStdString(task.remind_time));

    // print status
    string status;
    if (task.flag == true)
        status = "Incomplete";
    else
        status = "Complete";
    add_item << new QStandardItem(QString::fromStdString(status));
    m_model->appendRow(add_item);
}

User::~User()
{
    delete ui;
}

void User::back_to_login(void)
{
    Dialog *udialog = new Dialog;
    udialog->show();
    this->hide();
}

void User::add()
{
    this->hide();
    add_task *uadd = new add_task(this->_id, this->count, this->mytask);
    uadd->show();
}
