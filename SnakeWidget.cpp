#include "SnakeWidget.h"

#include <QPainter>
#include <QPoint>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>

#include <QtDebug>

SnakeWidget::SnakeWidget(QWidget *parent)
    : QWidget(parent),
      snake(new SnakeManager(this)),
      timer(new QTimer(this))
{
    timer->start(16);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    connect(this,SIGNAL(getNewPath(SnakeManager::path)),snake,SLOT(setNewPath(SnakeManager::path)));
    connect(snake,&SnakeManager::gameover,[](){
        QMessageBox::information(nullptr,"Gameover","Thanks for your play");
        std::exit(0);
    });

    setFixedSize(500,500);
}

void SnakeWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    default:break;
    case Qt::Key_Up:
    case Qt::Key_W:
        emit getNewPath(SnakeManager::goTop);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        emit getNewPath(SnakeManager::goDown);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        emit getNewPath(SnakeManager::goLeft);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        emit getNewPath(SnakeManager::goRight);
        break;
    }
    QWidget::keyPressEvent(event);
}

void SnakeWidget::displayRects(QPainter &painter,Qt::GlobalColor color,const QPolygon &points)
{
    painter.setBrush(QBrush(color,Qt::SolidPattern));
    QVector<QRect> rects;
    for(auto eachPoint:points)
    {
        QPoint tempPoint(eachPoint);
        tempPoint.setX(tempPoint.x()*20);
        tempPoint.setY(tempPoint.y()*20);
        QRect tempRect(tempPoint,QSize(20,20));
        rects<<tempRect;
    }
    painter.drawRects(rects);
}

void SnakeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    //back
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
    painter.drawRect(0,0,500,500);
    //snake
    displayRects(painter,Qt::white,snake->getSnakePoints());
    //obstacle
    displayRects(painter,Qt::red,snake->getObstaclePoints());
    //food
    displayRects(painter,Qt::yellow,QPolygon()<<snake->getFoodPoint());

    painter.end();

    QWidget::paintEvent(event);
}
