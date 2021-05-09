#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //QPoint offset;//存储鼠标与label位置差值
    int lastcellx=-1,lastcelly=-1;//存储鼠标移动事件中上一个细胞位置
    int startcellx=-1,startcelly=-1;//存储即将改变的矩形起点
    bool startcell=false;

protected:
    void paintEvent(QPaintEvent *event);
    void timerUpdate();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_startpushButton_clicked();
    void on_pausepushButton_clicked();
    void on_speedSpinBox_valueChanged(int arg1);
};


class map
{
private:
    int map[1000][1000];//存活状态 1为存活
    int neighbor[1000][1000];//邻居细胞个数
    int width=15; //细胞大小
    int height=15;
    int widthnum=60; //细胞个数
    int heightnum=40;

public:
    int getwidth(){return width;};
    int getheight(){return height;};
    void changewidth(int value){width=value;};
    void changeheight(int value){height=value;};
    int getwidthnum(){return widthnum;};
    int getheightnum(){return heightnum;};
    void changewidthnum(int value){widthnum=value;};
    void changeheightnum(int value){heightnum=value;};
    void changemap(int x,int y){map[x][y]^=1;};
    int getmap(int x,int y){return map[x][y];};
    void mapupdate();
};

#endif // MAINWINDOW_H
