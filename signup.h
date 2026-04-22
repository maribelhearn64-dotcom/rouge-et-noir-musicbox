#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>

namespace Ui {
class Signup;
}

class Signup : public QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

private slots:
    void on_pushButton_signup_clicked();

    void on_pushButton_tologin_clicked();

private:
    Ui::Signup *ui;

signals:
    void signal_sendmsg(QString u,QString p);
};

#endif // SIGNUP_H
