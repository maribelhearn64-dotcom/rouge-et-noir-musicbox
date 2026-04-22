#include "signup.h"
#include "ui_signup.h"
#include "login.h"
#include "mainwindow.h"
#include "dbmanager.h"
#include<QDebug>
#include<QMessageBox>
Signup::Signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_signup_clicked()
{
    //获取输入账号密码并且确认密码
    QString inputu = ui->lineEdit_u->text();
    QString inputp = ui->lineEdit_p->text();
    QString inputp2 = ui->lineEdit_p_2->text();
    //判空
    if(inputu.isEmpty()||inputp.isEmpty()||inputp2.isEmpty()){
        QMessageBox::warning(this,"SignUpWarning","username and password cannot be empty!");
        return ;
    }

    //数据库校验用户名是否存在
    if(Dbmanager::getDb()->userExist(inputu)){
    //用户名已经存在
        QMessageBox::warning(this,"UsernameWarning","username already exists!");
        ui->lineEdit_u->clear();
        return ;
    }
    //用户名未存在
    //判断后两次是否相同
    if(inputp != inputp2){
        QMessageBox::warning(this,"PasswordNotSameWarning","password's twice input not same!");
        ui->lineEdit_p->clear();
        ui->lineEdit_p_2->clear();
        return ;
    }
    //插入注册信息至用户表
    if(Dbmanager::getDb()->insertUser(inputu,inputp))
    //提示注册成功
        QMessageBox::information(this,"SignUpInformation","sign up success!");
    else{
        QMessageBox::warning(this,"SignUpInformation2","sign up failed!");
        return ;
    }
    //发出自定义信号，将用户名密码发送到login的输入框
    emit signal_sendmsg(inputu,inputp);
    //关闭注册窗口
    this->close();
}

void Signup::on_pushButton_tologin_clicked()
{
    Login* l = new Login(this);
    l->show();
}
