#ifndef ADD_TASK_H
#define ADD_TASK_H

#include <QWidget>
#include <class.h>
#include <QMessageBox>
namespace Ui {
class add_task;
}

class add_task : public QWidget
{
    Q_OBJECT

public:
    explicit add_task(string uid = "root", int ucount = 0, map<key,task> mytask = {}, QWidget *parent = 0);
    ~add_task();
public slots:
    void taskadd(void);
    void back_to_user(void);
private:
    Ui::add_task *ui;
    string _uid;
    int ucount;
    map<key,task> mytask;
};

#endif // ADD_TASK_H
