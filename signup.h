#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>

namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = 0);
    ~Signup();
public slots:
    void back_to_login(void);
    void add_user(void);
private:
    Ui::Signup *ui;
};

#endif // SIGNUP_H
