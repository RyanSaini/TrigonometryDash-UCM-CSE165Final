#include "Player.hpp"
#include <QGraphicsScene>

Player::Player(QPixmap p, int gp, int x, int y, QGraphicsItem* parent) : QGraphicsPixmapItem(parent){
    this->yDir = 0;
    setPixmap(p);

    fd =  275;

    groundPos = scenePos().y() + gp;
    setPos(x, y);

    this->yAnimation = new QPropertyAnimation(this,"y", this);
    this->fall();

    this->rotAnimation = new QPropertyAnimation(this, "rotation", this);

}

void Player::updatePixmap(){}

qreal Player::y() const
{
    return m_y;
}

void Player::setY(qreal y)
{
    moveBy(0, y - m_y);
    m_y = y;
}

qreal Player::rotation() const
{

    return m_rotation;
}

void Player::setRotation(qreal rotation)
{
    QPointF center = boundingRect().center(); // center point on QGraphicsPixmapItem
    QTransform t;
    t.translate(center.x(), center.y());
    t.rotate(rotation);
    t.translate(-center.x(), -center.y());
    setTransform(t);

    m_rotation = rotation;
}

void Player::jump(int x){
    this->yAnimation->stop();
    yAnimation->setStartValue(y());
    yAnimation->setEndValue(y() - (x + 100));
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(250-25);
    connect(yAnimation, &QPropertyAnimation::finished,[=](){
        this->fall();
    });
    yAnimation->start();
}

void Player::fall(){
    if(scenePos().y() > groundPos){
        setY(groundPos);
    }
    else{
        yAnimation->setStartValue(scenePos().y());
        yAnimation->setEndValue(groundPos);
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setDuration(fd);
        yAnimation->start();
        connect(yAnimation, &QPropertyAnimation::finished, [=](){
            this->canJump = true;
        });
    }

}

void Player::rotateTo(const qreal &end, const int &duration, const QEasingCurve &c)
{
    this->rotAnimation->stop();
    rotAnimation->setStartValue(rotation());
    rotAnimation->setEndValue(end);
    rotAnimation->setEasingCurve(c);
    rotAnimation->setDuration(duration);
    rotAnimation->start();
}

Player::~Player(){
    delete this->yAnimation;
}
