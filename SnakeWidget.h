#ifndef SNAKEWIDGET_H
#define SNAKEWIDGET_H

#include <QWidget>
#include "SnakeManager.h"

class QTimer;

class SnakeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SnakeWidget(QWidget *parent = 0);

signals:
    void getNewPath(SnakeManager::path newPath);

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void displayRects(QPainter &painter,Qt::GlobalColor color,const QPolygon &points);

    SnakeManager *snake;
    QTimer *timer;
};

#endif // SNAKEWIDGET_H
