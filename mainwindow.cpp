#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "dbmanager.h"
//目录操作：1.判断某个路径是否存在 2.创建文件的mkdir 3.提取目录下所有文件名
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMessageBox>
#include <set>
using namespace std;
MainWindow::MainWindow(QString username,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),user(username)
{
    ui->setupUi(this);
    QString nameshow = QString("Good day!dearest %1").arg(username);
    ui->label_4->setText(nameshow);
    //创建播放器对象
    player = new QMediaPlayer(this);
    //给播放器添加一首多媒体音乐
//    player->setMedia(QUrl::fromLocalFile("C:/Users/86131/Desktop/xujunpeng/cpp_in_QT/47/music/music/La gloire à mes genoux.mp3"));
//    //创建播放器列表对象
    list = new QMediaPlaylist(this);
    ui->pushButton_start_back->setIcon(QIcon(":/icon/icon/pri.png"));
    ui->pushButton_start_next->setIcon(QIcon(":/icon/icon/next.png"));
    //默认播放方式设置为循环播放
    //enum PlaybackMode { CurrentItemOnce, CurrentItemInLoop, Sequential, Loop, Random };单曲放一次 单曲循环 顺序 循环播放 随机播放
    list->setPlaybackMode(QMediaPlaylist::Loop);

    player->setVolume(ui->verticalSlider->value());

    connect(player,&QMediaPlayer::positionChanged,this,
            &MainWindow::slot_positionChanged);
    connect(list,&QMediaPlaylist::currentIndexChanged,
            this,&MainWindow::slot_currentIndexChanged);

    set<QString> hisInfos;
    Dbmanager::getDb()->selectHis(username,hisInfos);
    for(auto name:hisInfos){
        ui->listWidget_2->addItem(name);
    }
}
void MainWindow::slot_positionChanged(qint64 position)
{
    qDebug()<<"槽函数接收位置值"<<position;
    //当前播放哪首歌 滑动条最大值就是歌的长度
    qint64 maxlen = player->duration();
    qDebug()<<"歌曲总长度"<<maxlen;
    ui->horizontalSlider->setMaximum(maxlen);
    //参数position设置到水平滑动条上
    ui->horizontalSlider->setValue(position);
    //显示歌曲名称和歌曲时间
    //获取列表对象 当前播放歌曲的索引位置 按照索引位置去列表获取歌名
    int index = list->currentIndex();
    QListWidgetItem* item = ui->listWidget->item(index);
    if (item == nullptr)
        return;// 防止第二次点choose导致崩溃
    //获取指向行的文本内容
    QString musicname = item->text();
    ui->label->setText(musicname);

    //显示歌曲时间和总时长
    int musicmin = maxlen/1000/60;
    int musicsec = maxlen/1000%60;
    //当前播放时间
    int currentmin = position/1000/60;
    int currentsec = position/1000%60;
    //拼接字符串
    QString time = QString("%1:%2/%3:%4")
            .arg(currentmin, 2, 10, QChar('0'))   // 自动补0成两位
            .arg(currentsec, 2, 10, QChar('0'))
            .arg(musicmin,  2, 10, QChar('0'))
            .arg(musicsec,  2, 10, QChar('0'));
    ui->label_jindu->setText(time);

}

void MainWindow::slot_currentIndexChanged(int index)
{
    qDebug()<<"new song's index "<<index;
    //如果播放器没在播放 不处理历史记录
    if(player->state()!=QMediaPlayer::PlayingState){
        return ;
    }
    //寻找歌名
    QListWidgetItem* item = ui->listWidget->item(index);
    QString songname = item->text();
    //历史记录表查找是否已经存在这首歌（这一行）
    //findItem（）函数参数1：待查找内容 参数2：查找的匹配规则
    //返回值是一个容器 列表控件可以存多行元素
    //但此时我们的需求只需要一个元素
    QList<QListWidgetItem*> qlist = ui->listWidget_2->findItems(songname,Qt::MatchExactly);
    //如果找到了存在
    if(!qlist.isEmpty()){
        //1.删除现有的行
        //从容器中 获取找到内容的行索引
        QListWidgetItem* item = qlist.first();
        //从容器中 获取到找到内容的行索引
        int row = ui->listWidget_2->row(item);
        delete ui->listWidget_2->takeItem(row);
    }
    //如果没找到容器，在第一行插入
    else{
        Dbmanager::getDb()->insertHis(user,songname);
    }
    //insertItem（） 参数1：插入行的索引，0是第一行 参数2：插入行的显示内容
    ui->listWidget_2->insertItem(0,songname);

}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int row = ui->listWidget->row(item);
    list->setCurrentIndex(row);
    // 开始播放
        player->play();
        ui->pushButton_start->setIcon(QIcon(":/icon/icon/pause.png"));

}

void MainWindow::on_pushButton_clicked()
{
    player->stop();//先暂停当前歌曲

    ui->listWidget->clear();//清空列表
    list->clear();

    //四个参数 this 左上角的标题 点开后默认文件夹所在位置
    QString docupath = QFileDialog::getExistingDirectory(this,
    "choose directory","C:/Users/86131/Desktop/xujunpeng/cpp_in_QT/47/music");
    qDebug()<<docupath;
    QDir musicdir(docupath);
    auto musiclist = musicdir.entryList(QDir::Files,QDir::Name);
    for(auto iter = musiclist.begin();iter!=musiclist.end();iter++){
        if((*iter).endsWith(".mp3")){
            qDebug()<<*iter;
            ui->listWidget->addItem(*iter);
            //将当前的歌曲添加到歌曲列表里面
            list->addMedia(QUrl(docupath + "/" + *iter));

         }
    }
    //把添加后的列表发送给播放器
    player->setPlaylist(list);
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

}

void MainWindow::on_pushButton_start_clicked()
{
    if(player->state() == QMediaPlayer::StoppedState ||
    player->state() == QMediaPlayer::PausedState ){
        player->play();
        ui->pushButton_start->setIcon(QIcon(":/icon/icon/pause.png"));
    }
    else{
        player->pause();
        ui->pushButton_start->setIcon(QIcon(":/icon/icon/start.png"));
    }
    //如果没有进行切割 不会插入历史记录 手动插入这条记录
    slot_currentIndexChanged(list->currentIndex());
}


void MainWindow::on_pushButton_start_next_clicked()
{
    if(!list->mediaCount()){
        QMessageBox::information(this,"error","no music in here");
        return ;
    }
    list->next();
}

void MainWindow::on_pushButton_start_back_clicked()
{
    if(!list->mediaCount()){
        QMessageBox::information(this,"error","no music in here");
        return ;
    }
    list->previous();
}


//参数含义：下拉菜单更改了内容，新内容对应的索引行号
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch(index){
    case 0://loop
        list->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 1://random
        list->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case 2://list
        list->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    case 3://single
        list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    }
}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    qDebug()<<position;
    player->setVolume(position);
}



void MainWindow::on_horizontalSlider_sliderReleased()
{
    qint64 newposi = ui->horizontalSlider->value();

    player->setPosition(newposi);
}

void MainWindow::on_pushButton_switch_clicked()
{
    //弹窗确认是否切换账号
    int choose = QMessageBox::question(this,"switch account","do u really want to switch?",QMessageBox::Ok|QMessageBox::No);
    if(choose == QMessageBox::Ok){
    //确认——创建登录窗口
        player->stop();
        Login* l = new Login;
        l->show();
        this->close();}
    else
        return ;
}

void MainWindow::on_pushButton_2_clicked()
{
    //弹窗确认是否清空记录
    int choose = QMessageBox::question(this,"clear","do u really want to clear hisInfo?",QMessageBox::Ok|QMessageBox::No);
    if(choose == QMessageBox::Ok){
        Dbmanager::getDb()->deleteHis(user);
        ui->listWidget_2->clear();
    }
    else
        return ;

}

