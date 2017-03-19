#ifndef SNAKEMANAGER_H
#define SNAKEMANAGER_H

#include <QObject>
#include <QPolygon>

class QTimer;

class SnakeManager : public QObject
{
    Q_OBJECT
public:
    explicit SnakeManager(QObject *parent = 0);
    const QPolygon &getSnakePoints();
    const QPolygon &getObstaclePoints();
    const QPoint &getFoodPoint();

    static const quint32 x_max=25;
    static const quint32 y_max=25;

    enum path
    {
        goTop,
        goLeft,
        goRight,
        goDown,
    };

signals:
    void gameover();

public slots:
    void setNewPath(SnakeManager::path newPath);
    QPoint createFood();

private slots:
    void snakeMove();

private:
    void initMap();

    QPolygon snake;
    QPolygon obstacle;
    QPoint food;

    QTimer *timer;
    path nowPath;
};

#endif // SNAKEMANAGER_H
