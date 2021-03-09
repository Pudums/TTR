#include "View.h"
#include <QApplication>
#include <string>
#include <QBrush>
#include <QDesktopWidget>
#include <QGraphicsTextItem>
#include <iostream>
#include "Button.h"
#include "Wagon.h"
#include "WagonCard.h"
#include <QTextObject>

namespace {
std::map<std::string, int> color_to_sdvig = {
    {"White", 0},     {"Orange", 1},      {"Green", 2},  {"Red", 3},
    {"Black", 4},     {"Blue", 5},        {"Yellow", 6}, {"Purple", 7},
    {"Uncolored", 8}, {"Multicolored", 8}};
std::map<int, std::string> color_frow_owner = {
	{0, "Red"},
	{1, "Yellow"},
	{2, "Blue"},
	{3, "Green"}
};
}

View::View(QWidget *parrent) : Controller(new TTRController()) {
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
    int bxPos =
        this->width() / 2 - play_1_player_button->boundingRect().width() / 2;
    int byPos = 150;
    play_1_player_button->setPos(bxPos, byPos);
    connect(play_1_player_button, SIGNAL(clicked()), this,
            SLOT(start_player_1()));
    scene->addItem(play_1_player_button);

    Button *play_2_player_button = new Button(QString("2 Player"));
    bxPos =
        this->width() / 2 - play_2_player_button->boundingRect().width() / 2;
    byPos = 275;
    play_2_player_button->setPos(bxPos, byPos);
    connect(play_2_player_button, SIGNAL(clicked()), this,
            SLOT(start_player_2()));
    scene->addItem(play_2_player_button);

    Button *play_3_player_button = new Button(QString("3 Player"));
    bxPos =
        this->width() / 2 - play_3_player_button->boundingRect().width() / 2;
    byPos = 400;
    play_3_player_button->setPos(bxPos, byPos);
    connect(play_3_player_button, SIGNAL(clicked()), this,
            SLOT(start_player_3()));
    scene->addItem(play_3_player_button);

    Button *play_4_player_button = new Button(QString("4 Player"));
    bxPos =
        this->width() / 2 - play_4_player_button->boundingRect().width() / 2;
    byPos = 525;
    play_4_player_button->setPos(bxPos, byPos);
    connect(play_4_player_button, SIGNAL(clicked()), this,
            SLOT(start_player_4()));
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
    draw_map();
    draw_wagons();
    draw_players_cards();
	draw_active_cards();
}

void View::draw_map() {
    QBrush brush;
    brush.setTextureImage(QImage("data/map.jpg"));

    Button *map = new Button();
    map->setRect(0, 0, 1320, 880);
    map->set_clickable(false);
    map->setBrush(brush);
    scene->addItem(map);
}

void View::create_wagon(const WagonBlock &wagon, int owner) {
    QVector<QPointF> coords;
    for (const auto &point : wagon.coords.points) {
        coords << QPointF(point.x, point.y);
    }

    Wagon *wagon_to_draw = new Wagon(coords, owner != -1? color_frow_owner[owner] : "un_vis");

	connect(wagon_to_draw, &Wagon::clicked, [=]() {
			Controller->build_path_initialize(wagon.id);
			std::cout << "number wagon = " << wagon.id << '\n';
			draw_board();
	} );
    scene->addItem(wagon_to_draw);
}

void View::draw_wagons() {
    const auto &paths = Controller->get_paths();
    for (const auto &path : paths) {
		for (const auto &wagon : path.wagon_blocks) {
			create_wagon(wagon, path.owner);
		}
    }
}

void View::draw_deck() {
    QBrush q;
    q.setTextureImage(QImage("data/deck.jpeg"));

    Button *deck = new Button();
    int hight = 200, width = 327;  // free 1593 x 880
    deck->setRect(1920 - width, 1080 - hight, width, hight);
    deck->setBrush(q);
    connect(deck, SIGNAL(clicked()), this, SLOT(get_card_from_deck()));
    scene->addItem(deck);
}

void View::get_card_from_deck() {
	Controller->get_card_from_deck();
    draw_board();
}

void View::draw_players_cards() {
    const auto &cards = Controller->get_current_player_cards();
	std::map<std::string, int> count = Controller->get_count_by_color();
    for (const auto &card : cards) {
        int height = 150, width = 177;
        QVector<QPointF> coords;
        coords << QPointF(width * color_to_sdvig[card.color], 1080)
               << QPointF(width * color_to_sdvig[card.color], 1080 - height)
               << QPointF(width * (color_to_sdvig[card.color] + 1),
                          1080 - height)
               << QPointF(width * (color_to_sdvig[card.color] + 1), 1080);

        Wagon *wagon_to_draw = new Wagon(coords, card.color);
		connect(wagon_to_draw, &Wagon::clicked, [=]() {
				Controller->set_color_to_build_path(card);
				std::cout << "color = " << card.color << '\n';
				draw_board();
		} );
        scene->addItem(wagon_to_draw);

		QGraphicsTextItem* cur_color_count = new QGraphicsTextItem(QString::number(count[card.color]));
		QFont font("comic sans",50);
		cur_color_count->setFont(font);
		cur_color_count->setPos(width * (color_to_sdvig[card.color] + 0.5), 1080 - height);
		scene->addItem(cur_color_count);
    }
}

void View::draw_active_cards() {
	const auto &cards = Controller->get_active_cards();
	for(int i = 0; i < cards.size(); ++ i) {
		const auto & card = cards[i];
		int height = 176, width = 233;
        QVector<QPointF> coords;
        coords << QPointF(1920 - width, height * (i+1))
               << QPointF(1920 - width, height * (i))
               << QPointF(1920, height * (i))
               << QPointF(1920 , height * (i +1));
		Wagon *wagon_to_draw = new Wagon(coords, card.color);

		connect(wagon_to_draw, &Wagon::clicked, [=]() {
				Controller->get_card_from_active(i);
				std::cout << "color_active = " << card.color << '\n';
				draw_board();
		} );
		scene->addItem(wagon_to_draw);
	}
}
