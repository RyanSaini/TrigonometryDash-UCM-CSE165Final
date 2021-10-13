#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QDebug>
#include "Obstacle.hpp"
#include <QTimer>

class Player : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    explicit Player(QPixmap p, int gp, int x, int y, QGraphicsItem* parent = 0);
    ~Player();
    qreal y() const;
    qreal rotation() const;
    void jump(int x);
    QPropertyAnimation* yAnimation;
    bool canJump;
    qreal groundPos;
    int fd;

signals:
public slots:
    void setY(qreal y);
    void setRotation(qreal rotation);
    void fall();
    void rotateTo(const qreal &end, const int& duration, const QEasingCurve& c);

private:
    void updatePixmap();
    void setPositions(int xPos, int yPos);
    bool yDir; // 0 is DOWN, 1 is UP
    qreal m_y;
    qreal m_rotation;
    QPropertyAnimation* rotAnimation;
    int xPos, yPos;
    QTimer* timer;
};

#endif // PLAYER_HPP
