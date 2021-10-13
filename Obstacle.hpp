#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>
#include "Player.hpp"

class Obstacle : public QObject , public QGraphicsPixmapItem{
    Q_OBJECT //aniamtion properties as a QObject
    Q_PROPERTY(qreal x READ x WRITE setx)
public:
    explicit Obstacle(int xPos, int yPos, QString s, QGraphicsItem* parent = 0);
    qreal x() const; //getter
    int y() const; //getter
    void setPositions(int xPos, int yPos);
    void startAnimationsBlocks(int x);
    void startAnimationsFloor(int x);
    void startAnimationsBackground(int x);
    void startAnimationsTriangles(int x);
    void startAnimationsInvis(int x);
    QPropertyAnimation* xAnimation;
    void freeze();
    ~Obstacle();
signals:
    void collided();

public slots:
    void setx(qreal x); //setter

private:
    qreal m_x;
    int xPos, yPos;
    bool hasCollided();
};

class Floor_Box : public Obstacle{
public:
    Floor_Box(int xPos, int yPos, QString s, QGraphicsItem* parent = 0);
    void startAnimations(int x);
    ~Floor_Box();
};

class Triangle : public Obstacle{
public:
    Triangle(int xPos, int yPos, QString s, QGraphicsItem* parent = 0);
    void startAnimations(int x);
    ~Triangle();
};

class Background : public Obstacle{
public:
    Background(int xPos, int yPos, QString s, QGraphicsItem* parent = 0);
    void startAnimations(int x);
    ~Background();
};

class Floor : public Obstacle{
public:
    Floor(int xPos, int yPos, QString s, QGraphicsItem* parent = 0);
    void startAnimations(int x);
    ~Floor();
};

class Invis : public Obstacle{
public:
    Invis(int xPos, int yPos, QString s, QGraphicsItem* parent = 0);
    void startAnimations(int x);
    ~Invis();
};

#endif // OBSTACLE_H
