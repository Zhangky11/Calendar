#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <class.h>
namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT

public:
    explicit User(string user = "root", int count=0, map<key,task> mytask = {},QWidget *parent = 0);
    ~User();
public slots:
    void back_to_login(void);
    void add(void);
private:
    Ui::User *ui;
    string _id;
    int count=0;
    map<key,task> mytask;
    void display(task t, QStandardItemModel* m_model);
    void load_task_from_file(void);
};

// table
class DBTableView: public QTableView
{
public:
    explicit DBTableView(QWidget * _parent_widget = nullptr);
    ~DBTableView() override;

private:
    QStandardItemModel* db_table_model_;
};
#endif // USER_H
