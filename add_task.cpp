#include "add_task.h"
#include "ui_add_task.h"
#include "class.h"
#include "user.h"

extern mutex m; // for multithread use -- lock
string cmdseq_to_file_time(string input_str);

add_task::add_task(string _uid,int _ucount, map<key,task> mytask, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_task)
{
    ui->setupUi(this);
    this->_uid = _uid;
    this->ucount = _ucount;
    this->mytask = mytask;
}

add_task::~add_task()
{
    delete ui;
}
void add_task::back_to_user(void)
{
    User *usr = new User(this->_uid,this->ucount, this->mytask);
    usr->show();
    this->hide();
}
void add_task::taskadd(void)
{
    ofstream outfile;
    string _id = this->_uid;
    int count = this->ucount;
    map<key,task> mytask = this->mytask;

    string data_id = _id + ".data";
    outfile.open(data_id,ofstream::app);
    if(outfile.fail())
    {
        cerr<<"error：Failed to read the file."<<endl;
        assert(0);
    }
    /*
     文件已打开，开始执行任务录入，需要录入以下属性：
     string name;//任务名称
     int priority;//任务优先级 缺省值为低
     int type;//任务类型 缺省值为生活
     string remind_time;//提醒时间
     string start_time;//启动时间

     自动设置以下属性：
     bool flag;//是否已完成 完成为true
     bool remind_flag;//是否已提醒 已提醒为true
     int task_id;//唯一的index
    */
    task tmp_task;
    key tmp_key;

    string task_name = (ui->lineEdit->text()).toStdString();
    string task_type = (ui->comboBox->currentText()).toStdString();
    string task_prio = (ui->comboBox_2->currentText()).toStdString();
    string start_time = (ui->lineEdit_2->text()).toStdString();
    string remind_time = (ui->lineEdit_3->text()).toStdString();

    tmp_task.name = task_name;

    if(task_type=="Study")
        tmp_task.type=0;
    else if(task_type=="Entertainment")
        tmp_task.type=1;
    else
        tmp_task.type=2;

    if(task_prio=="High")
        tmp_task.priority=0;
    else if(task_prio=="Medium")
        tmp_task.priority=1;
    else
        tmp_task.priority=2;


    tmp_task.remind_time=cmdseq_to_file_time(remind_time);

    tmp_task.start_time=cmdseq_to_file_time(start_time);

    string tmp=tmp_task.start_time;

    string::size_type pos;
    string yyyy="",mm="",dd="",hh="",ff="",ss="00",other1="",other2="";

    pos=tmp.find("-");
    yyyy=tmp.substr(0,pos);
    other1=tmp.substr(pos+1);

    pos=other1.find("-");
    mm=other1.substr(0,pos);
    other2=other1.substr(pos+1);

    pos=other2.find(" ");
    dd=other2.substr(0,pos);
    other1=other2.substr(pos+1);

    pos=other1.find(":");
    hh=other1.substr(0,pos);
    other2=other1.substr(pos+1);

    pos=other2.find(":");
    ff=other2.substr(0,pos);
    other1=other2.substr(pos+1);

    count=count+1;
    std::string tmp_count = std::to_string(count);
    //    tmp_task.task_id=yyyy.append(mm).append(dd).append(hh).append(ff).append(ss).append(tmp_count);
    tmp_task.task_id=tmp_count;
    tmp_key.task_id=tmp_task.task_id;


    tmp_task.flag=false;
    tmp_task.remind_flag=false;

    //将tmp_task添加到mytask队尾
    unique_lock<mutex> g1(m, try_to_lock);
    if (g1.owns_lock())
    {
        mytask.insert(make_pair(tmp_key,tmp_task));
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Lock Error when add task!");
        msgBox.exec();
        return;
    }
    g1.unlock();

    //将任务tmp_task写入文件
    outfile<<tmp_task.task_id<<endl;//把任务id放在最前面，方便后续比较
    outfile<<tmp_task.name<<endl;
    outfile<<tmp_task.start_time<<endl;
    outfile<<tmp_task.type<<endl;
    outfile<<tmp_task.priority<<endl;
    outfile<<tmp_task.remind_time<<endl;
    if(tmp_task.flag==false)
        outfile<<"Incomplete"<<endl;
    else outfile<<"Completed"<<endl;
    outfile.close();
    QMessageBox msgBox;
    msgBox.setText("Insert task successfully");
    msgBox.exec();
    this->ucount = count;
    this->mytask = mytask;
    back_to_user();
}

string cmdseq_to_file_time(string input_str)
{
    string ans="2022-00-00 08:00:00";
    bool is_time=false;//标记用户是否输入当日具体时间

    string::size_type pos;
    pos=input_str.find("_");
    if (pos==input_str.npos)//没有输入hhmm
        is_time = false;
    else
        is_time = true;

    string ddmmyyyy="", hhmm="";
    if (is_time)
    {
        ddmmyyyy=input_str.substr(0,pos);
        hhmm=input_str.substr(pos+1);
    }
    else
    {
        ddmmyyyy=input_str;
    }

    ans[8]=ddmmyyyy[0];
    ans[9]=ddmmyyyy[1];

    ans[5]=ddmmyyyy[3];
    ans[6]=ddmmyyyy[4];


    if(ddmmyyyy.length()>5)
    {
        ans[0]=ddmmyyyy[6];
        ans[1]=ddmmyyyy[7];
        ans[2]=ddmmyyyy[8];
        ans[3]=ddmmyyyy[9];
    }

    if(is_time)
    {
        ans[11]=hhmm[0];
        ans[12]=hhmm[1];
        if(hhmm.length()>3)
        {
            ans[14]=hhmm[3];
            ans[15]=hhmm[4];
        }
    }
    return ans;
}
