#include "SnakeManager.h"

#include <QTimer>
#include <algorithm>
#include <random>
#include <ctime>

SnakeManager::SnakeManager(QObject *parent)
    : QObject(parent),
      timer(new QTimer(this))
{
    initMap();

    connect(timer,SIGNAL(timeout()),this,SLOT(snakeMove()));
    timer->start(200);
    nowPath=goTop;
}

void SnakeManager::initMap()
{
    for(int i=15;i!=20;++i)
    {
        snake.push_back(QPoint{20,i});
    }

    for(int i=0;i!=x_max;++i)
    {
        obstacle.push_back(QPoint{i,0});
        obstacle.push_back(QPoint{i,y_max-1});
    }
    for(int i=1;i!=y_max;++i)
    {
        obstacle.push_back(QPoint{0,i});
        obstacle.push_back(QPoint{x_max-1,i});
    }
    food=createFood();
}

void SnakeManager::snakeMove()
{
    QPoint nextHead;
    switch(nowPath)
    {
    default:break;
    case goTop:
        nextHead=QPoint(snake[0].x(),snake[0].y()-1);
        break;
    case goLeft:
        nextHead=QPoint(snake[0].x()-1,snake[0].y());
        break;
    case goRight:
        nextHead=QPoint(snake[0].x()+1,snake[0].y());
        break;
    case goDown:
        nextHead=QPoint(snake[0].x(),snake[0].y()+1);
        break;
    }
    if(nextHead==food)
    {
        snake.push_front(food);
        food=createFood();
        return;
    }

    auto selfIter=std::find_if(snake.begin()+1,snake.end(),[nextHead](const QPoint &eachPoint){
        return eachPoint==nextHead;
    });
    if(selfIter!=snake.end())
    {
        emit gameover();
        return;
    }
    auto obstacleIter=std::find_if(obstacle.begin(),obstacle.end(),[nextHead](const QPoint &eachPoint){
        return eachPoint==nextHead;
    });
    if(obstacleIter!=obstacle.end())
    {
        emit gameover();
        return;
    }

    snake.pop_back();
    snake.push_front(nextHead);
}

void SnakeManager::setNewPath(SnakeManager::path newPath)
{
    if(nowPath==newPath)
        return;
    switch(newPath)
    {
    default:break;
    case goTop:
        if(snake[0].y()-1==snake[1].y())
            return;
        break;
    case goLeft:
        if(snake[0].x()-1==snake[1].x())
            return;
        break;
    case goRight:
        if(snake[0].x()+1==snake[1].x())
            return;
        break;
    case goDown:
        if(snake[0].y()+1==snake[1].y())
            return;
        break;
    }

    nowPath=newPath;
}

const QPolygon& SnakeManager::getSnakePoints()
{
    return snake;
}

const QPolygon& SnakeManager::getObstaclePoints()
{
    return obstacle;
}

const QPoint& SnakeManager::getFoodPoint()
{
    return food;
}

QPoint SnakeManager::createFood()
{
    QPolygon emptyPoints;
    for(int i=1;i!=x_max-1;++i)
    {
        for(int j=1;j!=y_max-1;++j)
        {
            auto iter=std::find_if(snake.begin(),snake.end(),[i,j](const QPoint &snakePoint){
                return QPoint(i,j)==snakePoint;
            });
            if(iter==snake.end())
                emptyPoints.push_back(QPoint(i,j));
        }
    }

    static std::default_random_engine engine(std::time(nullptr));
    std::uniform_int_distribution<unsigned int> u(0,emptyPoints.size());
    auto location=u(engine);
    return QPoint(emptyPoints[location].x(),emptyPoints[location].y());
}
