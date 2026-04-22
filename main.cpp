#include "mainwindow.h"
#include "login.h"
#include "signup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;
    login.show();
    return a.exec();
}
