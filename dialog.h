#ifndef DIALOG_H
#define DIALOG_H
#include <QMessageBox>
#include <QDialog>
#include <user.h>
#include <signup.h>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void login_in(void);
    void sign_up(void);
    void quit(void);
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
