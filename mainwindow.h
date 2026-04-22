#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMediaPlayer>//播放器 开始 暂停 停止 显示播放进度位置
#include <QMediaPlaylist>//播放列表
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString username,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_start_clicked();

//    void on_pushButton_stop_clicked();


    void on_pushButton_start_next_clicked();

    void on_pushButton_start_back_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_verticalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_switch_clicked();

    void on_pushButton_2_clicked();
public slots:
    void slot_positionChanged(qint64 position);

    //处理切歌时的历史记录、
    void slot_currentIndexChanged(int index);
private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;//播放器对象指针
    QMediaPlaylist* list;//播放器列表对象指针
    QString user;//当前用户名
};
#endif // MAINWINDOW_H
