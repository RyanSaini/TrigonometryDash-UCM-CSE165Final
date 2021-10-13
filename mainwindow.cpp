#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
, ui(new Ui::MainWindow)
, myScene(new Scene())
, timer(new QTimer(this))
, deadTimer(new QTimer(this))
, collideCheckBox(true)
, collideCheckBox2(true)
, collideCheckBox3(true)
, collideCheckBox4(true)
, collideCheckFloor(true)

{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    this->setButtons();
    this->setGraphicsView(1920, 1080);
    this->setMenuScreen();
    this->loadGame();

    connect(this->timer, SIGNAL(timeout()), this, SLOT(setJump()));
    connect(this->deadTimer, SIGNAL(timeout()), this, SLOT(levelOneComplete()));

    gameAudio = new QMediaPlayer();
    gameAudio->setMedia(QUrl("qrc:/music/music/SHorter.mp3"));
    gameAudio->setVolume(50);
}

void MainWindow::setButtons(){
    connect(ui->startGame, SIGNAL(released()), this, SLOT(startGamePressed())); //Level 1 button
    ui->startGame->setStyleSheet("QPushButton { background-color: transparent; }");
    ui->startGame_2->setStyleSheet("QPushButton { background-color: transparent }");
}

void MainWindow::setGraphicsView(int width, int height){
    ui->graphicsView->setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setScene(myScene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFixedSize(width, height);
}

void MainWindow::setMenuScreen(){
    menuScreen  = new QGraphicsPixmapItem(QPixmap(":/images/images/final.png"));
    this->myScene->addItem(menuScreen);
}

void MainWindow::levelOneComplete(){
    this->myScene->clear();
    QGraphicsPixmapItem* end = new QGraphicsPixmapItem(QPixmap(":/images/images/GG.png"));
    this->myScene->addItem(end);
}

void MainWindow::initGame(){
    this->deadTimer->stop();
    this->myScene->clear();
    delete ui->startGame;
    delete ui->startGame_2;   
}

void MainWindow::loadGame(){
    deadTimer->start(35000);
    int i;
    int yInvis = 0;
    int xValue = 0;

    int yValueStack;

    for(i = 0; i < 900; i++){ //.................................................loading threeStack
        yValueStack = 705 -250;
        obstacle_x_locations.append(125 * i);
        Obstacle* o3 = new Obstacle(1920 + 125 * i, yValueStack, ":/images/images/threeStack.png");
        this->stack3.append(std::move(o3));
    }

    for(i = 0; i < 900; i++){ //...................................................loading twoStack
        yValueStack = 705 - 125;
        obstacle_x_locations.append(125 * i);
        Obstacle* o = new Obstacle(1920 + 125 * i, yValueStack, ":/images/images/twoStack.png");
        this->stack.append(std::move(o));
    }

    for(i = 0; i < 300; i++){ //.................................................loading invis
        obstacle_x_locations.append(i*300);
        if(i == 57){
            xValue = -150;
            yInvis = -300;
        }
        else if(i ==59){
            xValue = -100;
            yInvis = -200;

        }
        else if(i == 83){
            xValue = 50;
        }
        else if(i ==61){
            xValue = 50;
            yInvis = 150;

        }
        else if(i == 73){
            xValue = 50;
        }
        else if(i == 85){
            xValue = -450;
            yInvis = -200;
        }
        else if( i == 87) {
            xValue = -450;
        }
        else if (i == 81){
            yInvis = -200;
        }

        Invis* invis = new Invis((i*300) + 1920 + xValue, 400 - yInvis, ":/images/images/Diagonal.png");
        this->invis.append(std::move(invis));
        xValue = 0;
        yInvis = 0;
    }

    for(i = 0; i < 60; i++){ //.................................................loading floor
        obstacle_x_locations.append(i * 1920);
        Floor* floor = new Floor(0 + i * 1920, 830,":/images/images/floor.png");
        this->floor_.append(std::move(floor));
    }

    for(i = 0; i < 900; i++){ //..............................................loading triangles
        obstacle_x_locations.append(125 * i);
        Triangle* t = new Triangle((1920 + 125 * i), 830 - 125, ":/images/images/Triangle_White.png");
        this->triangles.append(std::move(t));
    }

    for(i = 0; i < 900; i++){ //..............................................loading floor_blocks
        obstacle_x_locations.append(i * 125);
        Floor_Box* o = new Floor_Box((1920 + 125 * i), 705, ":/images/images/Floor_Block.png");
        this->floor_blocks.append(std::move(o));
    }

    for(i = 0; i < 30; i++){ //.................................................loading background
        obstacle_x_locations.append(i * 1920);
        if(i % 2 == 0){
            Background* bg1 = new Background(i * 1920,0,":/images/images/Background_1.png");
            this->backgrounds.append(std::move(bg1));
        }
        else{
            Background* bg2 = new Background(i * 1920,0,":/images/images/Background_2.png");
            this->backgrounds.append(std::move(bg2));
        }
    }


    this->mapObstacles();
}

void MainWindow::startGamePressed(){
    gameAudio->play();
    if(menuScreen){
        delete menuScreen;
        menuScreen = 0;
    }
    initGame();
    int i;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    for(i = 0; i < this->backgrounds.size(); i++){ //............................drawing background
        this->myScene->addItem(this->backgrounds[i]);
        this->backgrounds[i]->startAnimationsBackground(obstacle_x_locations.back());
        obstacle_x_locations.pop_back();
    }

    for(i = 0; i < this->floor_blocks.size(); i++){ //.........................drawing floor_blocks
        if(box_map[i]){
            this->myScene->addItem(this->floor_blocks[i]);
            this->floor_blocks[i]->startAnimationsBlocks(obstacle_x_locations.back());
            connect(this->floor_blocks[i],&Obstacle::collided,[=](){
                if(collideCheckBox){
                    if(this->player->y() > this->floor_blocks[i]->y()){
                        this->stopAnimations();
                        this->gameAudio->stop();
                        this->myScene->removeItem(player);
                        delete player;
                        player = 0;
                        this->timer->stop();
                        this->deadTimer->stop();
                        QGraphicsPixmapItem *dead = new QGraphicsPixmapItem(QPixmap(":/images/images/dead.png"));
                        myScene->addItem(dead);
                    }
                    else{
                        delete this->player;
                        this->player = 0;
                        this->player = new Player(QPixmap(":/images/images/player.png"), 600, 150, 600);
                        this->myScene->addItem(player);
                        this->player->setPos(150, 600);
                        this->player->canJump = true;
                        this->canJump = true;
                        this->timer->stop();
                        collideCheckBox = false;
                        collideCheckFloor = true;
                        collideCheckBox2 = true;
                        collideCheckBox3 = true;
                    }
                }
            });
        }
        obstacle_x_locations.pop_back();
    }

    for(i = 0; i < this->triangles.size(); i++){ //........................drawing triangle obstacles
        if(triangle_map[i]){
                this->myScene->addItem(this->triangles[i]);
                this->triangles[i]->startAnimationsTriangles(obstacle_x_locations.back());
                connect(this->triangles[i], &Obstacle::collided,[=](){
                    gameAudio->stop();
                    qDebug() << "TRIANGLE COLLISION";
                    this->deadTimer->stop();
                    this->stopAnimations();
                    this->myScene->removeItem(this->player);
                    delete player;
                    player = 0;
                    this->timer->stop();
                    QGraphicsPixmapItem *dead = new QGraphicsPixmapItem(QPixmap(":/images/images/dead.png"));
                    this->myScene->addItem(dead);
                });
        }
        obstacle_x_locations.pop_back();
    }

    for(i = 0; i < this->floor_.size(); i++){ //..................................drawing floor
        if(floor_map[i]){
            this->myScene->addItem(this->floor_[i]);
            this->floor_[i]->startAnimationsFloor(obstacle_x_locations.back());
            connect(floor_[i], &Obstacle::collided,[=](){
                if(collideCheckFloor){
                    collideCheckFloor = false;
                    collideCheckBox = true;
                    collideCheckBox2 = true;
                    collideCheckBox3 = true;
                }
            });
        }
        obstacle_x_locations.pop_back();
    }

    for(int i = 0; i < this->invis.size(); i++){
        if(invis_map[i]){
            this->myScene->addItem(invis[i]);
            invis[i]->startAnimationsInvis(obstacle_x_locations.back());
            connect(invis[i], &Obstacle::collided, [=](){
                this->player->groundPos = 1080 - 250 - 124;
                this->player->fall();
            });
        }
        obstacle_x_locations.pop_back();
    }

    for(i = 0; i < this->stack.size(); i++){
        if(stack_map2[i]){
            this->myScene->addItem(this->stack[i]);
            this->stack[i]->startAnimationsBlocks(obstacle_x_locations.back());
            connect(this->stack[i],&Obstacle::collided,[=](){
                if(collideCheckBox2){
                    if(this->player->y() > this->stack[i]->y()){
                        this->gameAudio->stop();
                        this->stopAnimations();
                        this->myScene->removeItem(this->player);
                        delete player;
                        player = 0;
                        this->deadTimer->stop();
                        this->timer->stop();
                        QGraphicsPixmapItem *dead = new QGraphicsPixmapItem(QPixmap(":/images/images/dead.png"));
                        myScene->addItem(dead);
                    }
                    else{
                        delete this->player;
                        this->player = 0;
                        this->player = new Player(QPixmap(":/images/images/player.png"), 600 - 125, 150, 600 - 125);
                        this->myScene->addItem(player);
                        this->player->setPos(150, 600 - 125);
                        this->player->canJump = true;
                        this->canJump = true;
                        this->timer->stop();
                        collideCheckBox2 = false;
                        collideCheckBox = true;
                        collideCheckBox3 = true;
                        collideCheckFloor = true;
                   }
                }
            });
        }
        obstacle_x_locations.pop_back();
    }

    for(i = 0; i < this->stack3.size(); i++){
        if(stack_map3[i]){
            this->myScene->addItem(this->stack3[i]);
            this->stack3[i]->startAnimationsBlocks(obstacle_x_locations.back());
            connect(this->stack3[i],&Obstacle::collided,[=](){
                if(collideCheckBox3){
                    if(this->player->y() > this->stack3[i]->y()){
                        this->gameAudio->stop();
                        this->deadTimer->stop();
                        this->stopAnimations();
                        this->myScene->removeItem(this->player);
                        delete player;
                        player = 0;
                        this->timer->stop();
                        QGraphicsPixmapItem *dead = new QGraphicsPixmapItem(QPixmap(":/images/images/dead.png"));
                        myScene->addItem(dead);
                    }
                    else{
                        delete this->player;
                        this->player = 0;
                        this->player = new Player(QPixmap(":/images/images/player.png"), 600 - 250, 150, 600 - 250);
                        this->myScene->addItem(player);
                        this->player->setPos(150, 600 - 250);
                        this->player->canJump = true;
                        this->canJump = true;
                        this->timer->stop();
                        collideCheckBox3 = false;
                        collideCheckBox2 = true;
                        collideCheckBox = true;
                        collideCheckFloor = true;
                   }
                }
            });
        }
        obstacle_x_locations.pop_back();
    }

    this->player = new Player(QPixmap(":/images/images/player.png"), 1080 - 250 - 124, 150, 0);
    this->myScene->addItem(player);
    this->player->setPos(150, 0);
    this->timer->start(450); //timer to prevent jumps from happening too quickly
}

void MainWindow::stopAnimations(){
    QList<QGraphicsItem*> sceneItems = this->myScene->items();
    foreach(QGraphicsItem* item, sceneItems){
        Obstacle* o = dynamic_cast<Obstacle*>(item);
        if(o){
            o->xAnimation->stop();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setJump(){
    this->canJump = true;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    this->keys_hashmap[event->key()] = true;
    QMainWindow::keyPressEvent(event);
    if(event->key() == Qt::Key_Space && !event->isAutoRepeat() && this->keys_hashmap[event->key()] && this->canJump){
        this->timer->start(450);
        qDebug() << "SPACE PRESSED!";
        this->player->jump(150);
        this->canJump = false;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    this->keys_hashmap[event->key()] =  false;
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event){}

void MainWindow::mouseMoveEvent(QMouseEvent *event){}

void MainWindow::mapObstacles(){//i * .4166
    int i;
    for(i = 0; i < 30; i++){
        floor_map[i] = true;
    }
    triangle_map[6] = true;
    triangle_map[14] = true;
    triangle_map[15] = true;
    triangle_map[23] = true;
    triangle_map[40] = true;
    box_map[51] = true;
    box_map[50] = true;
    triangle_map[52] = true;
    triangle_map[53] = true;
    triangle_map[54] = true;
    invis_map[22] = true;
    triangle_map[67] = true;
    triangle_map[68] = true;
    triangle_map[77] = true;
    triangle_map[85] = true;
    triangle_map[86] = true;
    triangle_map[95] = true;
    box_map[120] = true;
    triangle_map[122] = true;
    box_map[126] = true;
    triangle_map[130] = true;
    triangle_map[128] = true;
    triangle_map[129] = true;
    invis_map[53] = true;
    box_map[140] = true;
    stack_map2[145] = true;
    stack_map3[150] = true;
    invis_map[57] = true;
    invis_map[59] = true;
    invis_map[61] = true;
    for(i = 151; i<155; i++){
        triangle_map[i] = true;
    }
    box_map[166] = true;
    triangle_map[167] = true;
    triangle_map[168] = true;
    invis_map[69] = true;
    triangle_map[175] = true;
    triangle_map[176] = true;
    box_map[177] = true;
    box_map[178] = true;
    box_map[179] = true;
    box_map[180] = true;
    stack_map2[181] = true;
    invis_map[73] = true;
    triangle_map[190] = true;
    box_map[200] = true;
    stack_map2[205] = true;
    box_map[207] = true;
    stack_map2[212] = true;
    box_map[214] = true;
    invis_map[81] = true;
    invis_map[83] =  true;
    invis_map[85] =  true;
    invis_map[87] =  true;
    triangle_map[225] = true;
    triangle_map[226] = true;
    triangle_map[234] = true;
    triangle_map[242] = true;
    triangle_map[243] = true;
    triangle_map[254] = true;
    triangle_map[255] = true;
    triangle_map[266] = true;
    triangle_map[267] = true;
    triangle_map[278] = true;
    triangle_map[279] = true;

    for(int i = 300; i < 900; i++){
        delete this->triangles[i];
        this->triangles[i] = 0;
        delete this->stack[i];
        this->stack[i] = 0;
        delete this->stack3[i];
        this->stack3[i] = 0;
        delete this->floor_blocks[i];
        this->floor_blocks[i] = 0;
    }

    for(int i = 100; i < 300; i++){
        delete this->invis[i];
        invis[i] = 0;
    }
}
