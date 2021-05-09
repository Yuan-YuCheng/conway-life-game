#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>

map map;//创建地图
QTimer *timer=new QTimer();
int speed=500;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(timer,&QTimer::timeout,this,&MainWindow::timerUpdate);
    timer->stop();
    ui->speedSpinBox->setValue(speed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//重新绘制
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //设置网格颜色
    QPen pen(Qt::gray);
    painter.setPen(pen);

    //以label左上角为原点
    painter.translate(ui->label->x(),ui->label->y());

    //清空画布
    QBrush brush(Qt::white,Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0,0,ui->label->width()-1,ui->label->height()-1);

    //绘制网格
    for(int x=0;x<=map.getwidthnum();x++)//绘制竖线
    {
        painter.drawLine(x*map.getwidth(),0,x*map.getwidth(),map.getheightnum()*map.getheight());
    }
    for(int y=0;y<=map.getheightnum();y++)//绘制横线
    {
        painter.drawLine(0,y*map.getheight(),map.getwidthnum()*map.getwidth(),y*map.getheight());
    }

    //绘制细胞
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    for(int x=0;x<map.getwidthnum();x++)
    {
        for(int y=0;y<map.getheightnum();y++)
        {
            if(map.getmap(x,y)==1)
                    painter.drawRect(x*map.getwidth(),y*map.getheight(),map.getwidth(),map.getheight());
        }
    }

    painter.end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QPoint point=event->pos()-ui->label->pos();
        //offset=event->pos()-ui->label->pos();
        if(point.x()>=0 and point.x()<=map.getwidth()*map.getwidthnum() and point.y()>=0 and point.y()<=map.getheight()*map.getheightnum())//防止超出map范围
        {
            lastcellx=point.x()/map.getwidth();
            lastcelly=point.y()/map.getheight();
            map.changemap(point.x()/map.getwidth(),point.y()/map.getheight());
            update();//重绘界面
        }
    }

    if(event->button()==Qt::RightButton)
    {
        if(startcell==false)//记录开始点坐标
        {
            startcell=true;
            startcellx=(event->pos().x()-ui->label->pos().x())/map.getwidth();
            startcelly=(event->pos().y()-ui->label->pos().y())/map.getheight();
        }
        else //绘制矩形
        {
            startcell=false;
            int stopcellx=(event->pos().x()-ui->label->pos().x())/map.getwidth();
            int stopcelly=(event->pos().y()-ui->label->pos().y())/map.getheight();
            if(startcellx>stopcellx) std::swap(startcellx,stopcellx);
            if(startcelly>stopcelly) std::swap(startcelly,stopcelly);
            for(int x=startcellx;x<=stopcellx;x++)
            {
                for(int y=startcelly;y<=stopcelly;y++)
                {
                    map.changemap(x,y);
                }
            }
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint point=event->pos()-ui->label->pos();
        //offset=event->pos()-ui->label->pos();
        if(point.x()>=0 and point.x()<=map.getwidth()*map.getwidthnum() and point.y()>=0 and point.y()<=map.getheight()*map.getheightnum())//防止超出map范围
        {
            if(lastcellx!=point.x()/map.getwidth() or lastcelly!=point.y()/map.getheight())
            {
                lastcellx=point.x()/map.getwidth();
                lastcelly=point.y()/map.getheight();
                map.changemap(point.x()/map.getwidth(),point.y()/map.getheight());
                update();//重绘界面
            }
        }
    }
}

void MainWindow::timerUpdate()
{
    map.mapupdate();
    update();//更新界面
}

void MainWindow::on_startpushButton_clicked()
{
    timer->start(speed);
}

void MainWindow::on_pausepushButton_clicked()
{
    timer->stop();
}

void MainWindow::on_speedSpinBox_valueChanged(int arg1)
{
    speed=arg1;
    if(timer->isActive())
        timer->start(speed);
}

//map---------------------------------------------------------------------------------------------------------

void map::mapupdate()
{
    int dx[8]={0,0,-1,1,-1,1,-1,1};
    int dy[8]={1,-1,0,0,1,1,-1,-1};

    memset(neighbor,0,sizeof(neighbor));//邻居细胞数量清零

    //重新统计邻居细胞数量
    for(int x=0;x<widthnum;x++)
    {
        for(int y=0;y<heightnum;y++)
        {
            if(map[x][y]==1)
            {
                for(int i=0;i<8;i++)
                {
                    if(dx[i]+x>=0 and dx[i]+x<=widthnum and dy[i]+y>=0 and dy[i]+y<=heightnum) //防止越界
                    {
                        neighbor[dx[i]+x][dy[i]+y]++;
                    }
                }
            }
        }
    }

    //改变细胞状态
    for(int x=0;x<widthnum;x++)
    {
        for(int y=0;y<heightnum;y++)
        {
            if(neighbor[x][y]==3)
                map[x][y]=1;
            else if(neighbor[x][y]!=2)
                map[x][y]=0;
        }
    }
}
