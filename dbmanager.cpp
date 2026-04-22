#include "dbmanager.h"
#include <set>
//静态变量类外初始化
Dbmanager* Dbmanager::p = new Dbmanager;//饿汉式

Dbmanager::Dbmanager()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("music.db");//设置数据库名字
    db.open();//打开数据库
    query = (QSqlQuery)db;//query与db数据库绑定
    //创建用户表的语句 user_info
    /*id        integer primary key autoincrement
     * username
     * password
    */
    QString sqluser = "create table if not exists user_info(id integer primary key autoincrement,username varchar(30),password varchar(30));";
    //创建历史记录表的语句 his_info
    /* username
     * musicname
     * showtime(听歌时间)   datetime(时间类型)
    */
    QString sqlhistory = "create table if not exists his_info(username varchar(30),musicname varchar(50),showtime datetime);";
    query.exec(sqluser);
    query.exec(sqlhistory);
}

Dbmanager::Dbmanager(const Dbmanager &)
{

}

Dbmanager *Dbmanager::getDb()
{
    return p;
}

bool Dbmanager::userExist(QString name)
{
    int count = 0;
    //执行查询语句 是否存在与name相同的用户
    query.prepare("select count(*) from user_info where username = ?");
    query.bindValue(0,name);
    query.exec();
    //提取查询结果
    while(query.next()){
        count = query.value(0).toInt();
    }
    return count;
}

bool Dbmanager::insertUser(QString name, QString pass)
{   //执行插入语句
    query.prepare("insert into user_info (username,password) values (?,?);");
    query.bindValue(0,name);
    query.bindValue(1,pass);
    bool ok = query.exec();
    if(ok){
        qDebug()<<"insert succ";
        return 1;}
    else{
        qDebug()<<"insert failed";
        return 0;}
}

bool Dbmanager::loginUser(QString name, QString pass)
{
    query.prepare("select password from user_info where username = ?;");
    query.bindValue(0,name);
    query.exec();
    if (query.next()) {
            QString truepass = query.value(0).toString();
            return (truepass == pass);      // 密码相同返回 true
        }
    return false;
}

bool Dbmanager::insertHis(QString username,QString musicname)
{
    query.prepare("insert into his_info values(?,?,datetime('now','localtime'));");
    query.bindValue(0,username);
    query.bindValue(1,musicname);
    bool ok = query.exec();
    if(ok){
        qDebug()<<"insert succ"; return 1; }
    else{
        qDebug()<<"insert fail"; return 0;}
}



bool Dbmanager::updateHis(QString username, QString musicname)
{
    query.prepare("update his_info values(?,?,datetime('now','localtime'));");
    query.bindValue(0,username);
    query.bindValue(1,musicname);
    bool ok = query.exec();
    if(ok){
        qDebug()<<"update succ"; return 1; }
    else{
        qDebug()<<"update fail"; return 0;}
}
bool Dbmanager::selectHis(QString username,set<QString> &infos)
{
    query.prepare("select musicname from his_info where username = ?;");
    query.bindValue(0,username);
    query.exec();
    while(query.next()){
        QString musicname = query.value(0).toString();
        infos.insert(musicname);
    }
}

bool Dbmanager::deleteHis(QString username)
{
    query.prepare("delete from his_info where username = ?;");
    query.bindValue(0,username);
    bool ok = query.exec();
    if(ok){
        qDebug()<<"delete succ"; return 1; }
    else{
        qDebug()<<"delete fail"; return 0;}
}
