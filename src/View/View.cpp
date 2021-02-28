#include "View.h"
#include "Button.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsTextItem>
#include <iostream>

View::View(QWidget *parrent, TTRController *contr): Controller(contr) {
    int screen_width = 1920, screen_height = 1080;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(screen_width, screen_height);

    this->setWindowState(Qt::WindowFullScreen);
    this->showFullScreen();
    fitInView(QApplication::desktop()->availableGeometry(-1),
              Qt::IgnoreAspectRatio);

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screen_width, screen_height);
    setScene(scene);
}

void View::display_menu() {
    QGraphicsTextItem *titleText =
        new QGraphicsTextItem(QString("Ticket to Ride"));
    QFont titleFont("comic sans", 50);
    titleText->setFont(titleFont);
    int txPos = this->width() / 2 - titleText->boundingRect().width() / 2;
    int tyPos = 150;
    titleText->setPos(txPos, tyPos);
    scene->addItem(titleText);

    Button *playButton = new Button(QString("Play"));
    int bxPos = this->width() / 2 - playButton->boundingRect().width() / 2;
    int byPos = 275;
    playButton->setPos(bxPos, byPos);
    connect(playButton, SIGNAL(clicked()), this, SLOT(start()));
    scene->addItem(playButton);

    Button *quitButton = new Button(QString("Quit"));
    int qxPos = this->width() / 2 - quitButton->boundingRect().width() / 2;
    int qyPos = 350;
    quitButton->setPos(qxPos, qyPos);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    scene->addItem(quitButton);
}

void View::start() {
	scene->clear();
    Button *play_1_player_button = new Button(QString("1 Player"));
    int bxPos = this->width()/2-play_1_player_button->boundingRect().width()/2;
    int byPos = 150;
    play_1_player_button->setPos(bxPos, byPos);
    connect(play_1_player_button, SIGNAL(clicked()),this,SLOT(count_of_players(1)));
    scene->addItem(play_1_player_button);
}
