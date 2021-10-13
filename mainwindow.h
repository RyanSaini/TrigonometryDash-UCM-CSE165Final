#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QScopedPointer>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QEvent>
#include <QTimer>
#include <QMediaPlayer>

#include "Scene.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent*);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

public slots:


private:
    Ui::MainWindow *ui;
    Scene* myScene;
    Player* player;
    QVector<int> obstacle_x_locations;
    QVector<Obstacle*> floor_blocks, floor_, backgrounds, triangles, invis, stack, stack3, stack4;
    void setButtons();
    void mapObstacles();
    void setGraphicsView(int width, int height);
    void setMenuScreen();
    void stopAnimations();
    void initGame();
    void loadGame();
    QTimer* timer, *deadTimer;
    bool collideCheckBox, collideCheckBox2, collideCheckBox3, collideCheckBox4,  collideCheckFloor, canJump;
    QMap<int, bool> keys_hashmap, triangle_map, box_map, floor_map, invis_map, stack_map2, stack_map3, stack_map4;
    QGraphicsPixmapItem* menuScreen;

    QMediaPlayer* gameAudio;


private slots:
     void startGamePressed();
     void setJump();
     void levelOneComplete();

};
#endif // MAINWINDOW_H
