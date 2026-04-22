#include "login.h"
#include "ui_login.h"
#include "signup.h"
#include "mainwindow.h"
#include "dbmanager.h"
#include <QDebug>
#include <QMessageBox>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    //获取输入用户密码
    QString inputusername = ui->lineEdit_u->text();
    QString inputpassword = ui->lineEdit_p->text();
    //判空
    if(inputusername.isEmpty()||inputpassword.isEmpty()){
        qDebug()<<"plz input something";
        QMessageBox::warning(this,"LoginWarning","username and password cannot be empty!");
        return ;
    }
    //调用数据库进行校验
    if(Dbmanager::getDb()->userExist(inputusername)){
        if(!Dbmanager::getDb()->loginUser(inputusername,inputpassword)){
            QMessageBox::critical(this,"Wrong","password incorrect!!!");
            ui->lineEdit_p->clear();
            return ;
        }
    }
    else{
        QMessageBox::warning(this,"LoginWarning2","username not exists");
        ui->lineEdit_p->clear();
        ui->lineEdit_u->clear();
        return ;
    }
    //创建播放器窗口对象
    MainWindow* m = new MainWindow( inputusername );
    //显示窗口，关闭当前窗口
    m->show();
    this->close();
}

void Login::on_pushButton_tosignup_clicked()
{
    //创建注册窗口对象
    Signup* s = new Signup();
    s->show();
    connect(s,&Signup::signal_sendmsg,this,&Login::slot_recvmsg);
}

void Login::slot_recvmsg(QString u, QString p)
{
    qDebug() << "slot_recvmsg called with:" << u << p;
     ui->lineEdit_u->setText(u);
     ui->lineEdit_p->setText(p);
}
