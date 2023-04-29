#ifndef USER_H
#define USER_H

#include <QWidget>
#include <class.h>
namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT

public:
    explicit User(string user = "root", QWidget *parent = 0);
    ~User();

private:
    Ui::User *ui;
};

#endif // USER_H
