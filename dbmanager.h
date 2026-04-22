#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <set>
using namespace std;
//数据库功能相关类
//保证整个项目只存在一个数据库，采用 单例模式
class Dbmanager{
    //私有化 构造函数 拷贝构造函数
    Dbmanager();
    Dbmanager(const Dbmanager&);
    //静态的 指向唯一实例化对象的指针
    static Dbmanager* p;
    //数据库对象
    QSqlDatabase db;
    QSqlQuery query;
public:
    //静态函数返回指针
    static Dbmanager* getDb();
    //注册功能
    //1.校验用户名是否存在
    bool userExist(QString name);
    //2.向用户表中插入数据
    bool insertUser(QString name,QString pass);
    //登录功能
    //同时校验满足用户名和密码匹配的用户有几个
    bool loginUser(QString name,QString pass);
     //历史记录功能
    //1.向历史表插入数据
    bool insertHis(QString username,QString musicname);
    //2.切歌时 更新历史记录时间
    bool updateHis(QString username,QString musicname);
    //3.登录后 提取所有当前用户历史记录
    bool selectHis(QString username,set<QString> &infos);
    //4.清除本账号的历史记录
    bool deleteHis(QString username);
};

#endif // DBMANAGER_H
