#include "View.h"
#include "Button.h"
#include <iostream> // TODO
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsTextItem>
#include <iostream>

View::View(QWidget *parrent): Controller(new TTRController()) {
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
    connect(play_1_player_button, SIGNAL(clicked()),this,SLOT(start_player_1()));
    scene->addItem(play_1_player_button);

    Button *play_2_player_button = new Button(QString("2 Player"));
    bxPos = this->width()/2-play_2_player_button->boundingRect().width()/2;
    byPos = 275;
    play_2_player_button->setPos(bxPos, byPos);
    connect(play_2_player_button, SIGNAL(clicked()),this,SLOT(start_player_2()));
    scene->addItem(play_2_player_button);

    Button *play_3_player_button = new Button(QString("3 Player"));
    bxPos = this->width()/2-play_3_player_button->boundingRect().width()/2;
    byPos = 400;
    play_3_player_button->setPos(bxPos, byPos);
    connect(play_3_player_button, SIGNAL(clicked()),this,SLOT(start_player_3()));
    scene->addItem(play_3_player_button);

    Button *play_4_player_button = new Button(QString("4 Player"));
    bxPos = this->width()/2-play_4_player_button->boundingRect().width()/2;
    byPos = 525;
    play_4_player_button->setPos(bxPos, byPos);
    connect(play_4_player_button, SIGNAL(clicked()),this,SLOT(start_player_4()));
    scene->addItem(play_4_player_button);
}

void View::start_player_1() {
	Controller->start_game(1);
	draw_board();
}

void View::start_player_2() {
	Controller->start_game(2);
	draw_board();
}

void View::start_player_3() {
	Controller->start_game(3);
	draw_board();
}

void View::start_player_4() {
	Controller->start_game(4);
	draw_board();
}

void View::draw_board() {
	scene->clear();
	draw_deck();
}

void View::draw_deck() {
	QString fileName = "data/deck.jpeg";
	QImage image(fileName);
	scene->addPixmap( QPixmap::fromImage(image));
	/*
	QImage *deck_image = new QImage();
	deck_image->load("data/deck.jpeg");
	scene->addItem(deck_image);
	*/
}
