#include "Obstacle.hpp"
#include <QDebug>
#include <QGraphicsScene>

Obstacle::Obstacle(int xPos, int yPos, QString s, QGraphicsItem* parent) : QGraphicsPixmapItem(parent){
    QPixmap obs = QPixmap(s);
    setPixmap(obs);
    this->xAnimation = new QPropertyAnimation(this, "x", this);
    this->setPositions(xPos, yPos);
    connect(xAnimation, &QPropertyAnimation::finished, [=](){
        //qDebug() << "ANIMATION FINSIHED";
        scene()->removeItem(this);
        delete this;
    });
}

qreal Obstacle::x() const //getter
{
    return m_x;
}
int Obstacle::y() const
{
    return this->yPos;
}

void Obstacle::setx(qreal x) //setter
{
    if(hasCollided()){
        emit this->collided();
    }
    m_x = x;
    setPos(QPointF(0,0) + QPointF(x, this->yPos));
}

void Obstacle::setPositions(int xPos, int yPos){
    this->xPos = xPos;
    this->yPos = yPos;
    setPos(QPoint(0,0) + QPoint(this->xPos, this->yPos));
}

//SPEED OF FLOOR + BLOCKS + OBSTACLES CURRENTLY AT 1.518 * 2 = 3.036 units / ms

void Obstacle::startAnimationsFloor(int x){ //we are creating 20 floors. Multiply the width of the floor by 20.... (1920* 20) = 38400. Now you can divide distance by speed to get time, r = d / t
    this->xAnimation->setStartValue(this->xPos);
    this->xAnimation->setEndValue(-x - 1920);
    this->xAnimation->setEasingCurve(QEasingCurve::Linear);
    this->xAnimation->setDuration(25297*3.5);
    this->xAnimation->start();
}

void Obstacle::startAnimationsBackground(int x){
    this->xAnimation->setStartValue(this->xPos);
    this->xAnimation->setEndValue(-x - 1920);
    this->xAnimation->setEasingCurve(QEasingCurve::Linear);
    this->xAnimation->setDuration(300000*3.5);
    this->xAnimation->start();
    connect(this->xAnimation, &QPropertyAnimation::finished,[=](){
        delete this;
    });
}

void Obstacle::startAnimationsBlocks(int x){
    this->xAnimation->setStartValue(this->xPos);
    this->xAnimation->setEndValue(-x - 125);
    this->xAnimation->setEasingCurve(QEasingCurve::Linear);
    this->xAnimation->setDuration(24622*3.5);
    this->xAnimation->start();
    connect(this->xAnimation, &QPropertyAnimation::finished,[=](){
        delete this;
    });
}

void Obstacle::startAnimationsTriangles(int x){
    this->xAnimation->setStartValue(this->xPos);
    this->xAnimation->setEndValue(-x - 125);
    this->xAnimation->setEasingCurve(QEasingCurve::Linear);
    this->xAnimation->setDuration(24622*3.5);
    this->xAnimation->start();
    connect(this->xAnimation, &QPropertyAnimation::finished,[=](){
        delete this;
    });
}

void Obstacle::startAnimationsInvis(int x){
    this->xAnimation->setStartValue(this->xPos);
    this->xAnimation->setEndValue(-x - 60);
    this->xAnimation->setEasingCurve(QEasingCurve::Linear);
    this->xAnimation->setDuration(11857 * 6);
    this->xAnimation->start();
    connect(this->xAnimation, &QPropertyAnimation::finished,[=](){
        delete this;
    });
}

Obstacle::~Obstacle(){}

bool Obstacle::hasCollided(){
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    foreach (QGraphicsItem* item, collidingItems){
        Player* player = dynamic_cast<Player*>(item);
        if(player){
            return true;
        }
    }
    return false;
}

void Obstacle::freeze(){
//
}

Floor_Box::Floor_Box(int xPos, int yPos, QString s, QGraphicsItem* parent) : Obstacle(xPos, yPos, s, parent){}
void Floor_Box::startAnimations(int x){ Obstacle::startAnimationsBlocks(x); }
Floor_Box::~Floor_Box(){ delete this->xAnimation; }

Triangle::Triangle(int xPos, int yPos, QString s, QGraphicsItem* parent) : Obstacle(xPos, yPos, s, parent){}
void Triangle::startAnimations(int x){ Obstacle::startAnimationsTriangles(x); }
Triangle::~Triangle(){ delete this->xAnimation; }

Background::Background(int xPos, int yPos, QString s, QGraphicsItem* parent) : Obstacle(xPos, yPos, s, parent){}
void Background::startAnimations(int x){ Obstacle::startAnimationsBackground(x); }
Background::~Background(){ delete this->xAnimation; }

Floor::Floor(int xPos, int yPos, QString s, QGraphicsItem* parent) : Obstacle(xPos, yPos, s, parent){}
void Floor::startAnimations(int x){ Obstacle::startAnimationsFloor(x); }
Floor::~Floor(){ delete this->xAnimation; }

Invis::Invis(int xPos, int yPos, QString s, QGraphicsItem* parent) : Obstacle(xPos, yPos, s, parent){}
void Invis::startAnimations(int x){ Obstacle::startAnimationsFloor(x); }
Invis::~Invis(){ delete this->xAnimation; }
