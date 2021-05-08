#include "View.h"
#include "CircleWidget.h"
#include "Station.h"
#include "Button.h"
#include "Wagon.h"
#include "WagonCard.h"
#include <QPalette>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QApplication>
#include <string>
#include <QBrush>
#include <QDesktopWidget>
#include <QGraphicsTextItem>
#include <iostream>
#include <QTextObject>
#include <QGraphicsProxyWidget>

namespace {
unsigned int microseconds = 1000;

std::map<std::string, int> color_to_sdvig = {
    {White, 0},     {Orange, 1},      {Green, 2},  {Red, 3},
    {Black, 4},     {Blue, 5},        {Yellow, 6}, {Purple, 7},
    {Uncolored, 8}, {Multicolored, 8}};

std::map<int, std::string> color_frow_owner = {
	{0, Red},
	{1, Yellow},
	{2, Blue},
	{3, Green}
};

qreal deck_z_vzlue = 0,
	  map_z_value = 0,
	  wagons_invisible_z_value= 1, // don't work, cause lay under map and become unclicable
	  wagons_visible_z_value = 1,
	  players_cards_z_value = 0,
	  active_card_z_value = 0,
	  animation_z_value = -1;
}

void View::draw_stations() {
	const auto &stations = Controller->get_stations();
	for(auto station: stations) {
		const auto &s = station.second;
		const auto &name = station.first;

		Station *path = new Station;
		path->addEllipse(QPointF(s.p.x, s.p.y) , 20, 20);
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(Qt::cyan);
		QPen *pen = new QPen;
		pen->setBrush(brush);
		connect(path, &Station::clicked, [=]() {
				std::cout << "station " << name << " cliced\n";
		} );
		scene->addPath(*path, *pen, brush);
	}
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

    Button *playOnlineButton = new Button(QString("Play Online"));
    bxPos = this->width() / 2 - playOnlineButton->boundingRect().width() / 2;
    byPos = 350;
    playOnlineButton->setPos(bxPos, byPos);
    connect(playOnlineButton, &Button::clicked, [=](){
			});
    scene->addItem(playOnlineButton);

    Button *quitButton = new Button(QString("Quit"));
    int qxPos = this->width() / 2 - quitButton->boundingRect().width() / 2;
    int qyPos = 350 + 125;
    quitButton->setPos(qxPos, qyPos);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    scene->addItem(quitButton);
}

void View::start(bool is_server) {
    scene->clear();

    Button *play_1_player_button = new Button(QString("1 Player"));
    int bxPos =
        this->width() / 2 - play_1_player_button->boundingRect().width() / 2;
    int byPos = 150;
    play_1_player_button->setPos(bxPos, byPos);
    connect(play_1_player_button, &Button::clicked, [=]() {
			start_player_1(is_server);
		});
    scene->addItem(play_1_player_button);

    Button *play_2_player_button = new Button(QString("2 Player"));
    bxPos =
        this->width() / 2 - play_2_player_button->boundingRect().width() / 2;
    byPos = 300;
    play_2_player_button->setPos(bxPos, byPos);
    connect(play_2_player_button, &Button::clicked, [=]() {
			start_player_2(is_server);
		});
    scene->addItem(play_2_player_button);

    Button *play_3_player_button = new Button(QString("3 Player"));
    bxPos =
        this->width() / 2 - play_3_player_button->boundingRect().width() / 2;
    byPos = 450;
    play_3_player_button->setPos(bxPos, byPos);
    connect(play_3_player_button, &Button::clicked, [=]() {
			start_player_3(is_server);
		});
    scene->addItem(play_3_player_button);

    Button *play_4_player_button = new Button(QString("4 Player"));
    bxPos =
        this->width() / 2 - play_4_player_button->boundingRect().width() / 2;
    byPos = 600;
    play_4_player_button->setPos(bxPos, byPos);
    connect(play_4_player_button, &Button::clicked, [=]() {
			start_player_4(is_server);
		});
    scene->addItem(play_4_player_button);
}

void View::start_player_1(bool is_server) {
	choose_count_of_bots(1, is_server);
}

void View::start_player_2(bool is_server) {
	choose_count_of_bots(2, is_server);
}

void View::start_player_3(bool is_server) {
	choose_count_of_bots(3, is_server);
}

void View::start_player_4(bool is_server) {
	choose_count_of_bots(4, is_server);
}

void View::choose_count_of_bots(int n, bool is_server) {
    scene->clear();

	for(int i = 0; i <= n; i++) {
		Button *bot = new Button(QString::number(i) + QString(" Bots"));
		int bxPos =
			this->width() / 2 - bot->boundingRect().width() / 2;
		int byPos = 150 * (i + 1);
		bot->setPos(bxPos, byPos);
		connect(bot, &Button::clicked, [=]() {
				Controller->start_game(n, i, is_server);
				draw_board();
		} );
		scene->addItem(bot);
	}
}

void View::draw_board() {
    scene->clear();
	auto status = Controller->is_game_end();
	if(status == 2) {
		Controller->end_game();
		end_game();
		return;
	} else {
		draw_deck();
		draw_map();
		draw_wagons();
		draw_players_cards();
		draw_active_cards();
		draw_wagons_count();
		draw_stations();
	}
}

void View::draw_wagons_count() {
	const auto players = Controller->get_players();
	int height = 220, width = 367;
    for (int i = 0; i < players.size(); ++ i) {
		const auto &player = players[i];
        QVector<QPointF> coords;
        coords << QPointF(1320 + width * 0, height * i)
               << QPointF(1320 + width * 1, height * i)
               << QPointF(1320 + width * 1, height * (i + 1))
               << QPointF(1320 + width * 0, height * (i + 1));

        Wagon *wagon_to_draw = new Wagon(coords, 
				color_frow_owner[player.id]);
        scene->addItem(wagon_to_draw);

		QFont font("comic sans", 14);
		QGraphicsTextItem* some_text1 = new QGraphicsTextItem(QString("Wagons left: ") + QString::number(player.number_of_wagons_left));
		some_text1->setFont(font);
		some_text1->setPos(1320 + width * 0.3, i * height);
		scene->addItem(some_text1);

		QGraphicsTextItem* some_text2 = new QGraphicsTextItem(QString("Points: ") 
				+ QString::number(player.points));
		some_text2->setPos(1320 + width * 0.3, i * height + 30);
		some_text2->setFont(font);
		scene->addItem(some_text2);

		QGraphicsTextItem* some_text3 = new QGraphicsTextItem(QString("Stations left: ") 
				+ QString::number(player.number_of_stations_left));
		some_text3->setPos(1320 + width * 0.3, i * height + 30 * 2);
		some_text3->setFont(font);
		scene->addItem(some_text3);

		QGraphicsTextItem* some_text4 = new QGraphicsTextItem(QString("Cards : ") 
				+ QString::number(player.wagon_cards.size()));
		some_text4->setPos(1320 + width * 0.3, i * height + 30 * 3);
		some_text4->setFont(font);
		scene->addItem(some_text4);
		bool flag = true;

		connect(wagon_to_draw, &Wagon::clicked, [=, flag]() mutable {
				scene->removeItem(wagon_to_draw);
				scene->addItem(wagon_to_draw);
				if(flag) {
					scene->removeItem(some_text1);
					scene->removeItem(some_text2);
					scene->removeItem(some_text3);
					scene->removeItem(some_text4);
					int j = 0;
					for(auto &path: player.active_routes) {
						QGraphicsTextItem *some_text = new QGraphicsTextItem(QString("From: ") 
							+ QString(path.city1.c_str()) + QString(" to ") + QString(path.city2.c_str()));
						some_text->setPos(1320 + width * 0.05, i * height + 25 * (2 * j));
						some_text->setFont(font);
						scene->addItem(some_text);

						some_text = new QGraphicsTextItem(QString("Left to build: ") 
							+ QString::number(path.points_for_passing));
						some_text->setPos(1320 + width * 0.05, i * height + 25 * (2 * j + 1));
						some_text->setFont(font);
						scene->addItem(some_text);
						j++;
					}
				}else {
					scene->addItem(some_text1);
					scene->addItem(some_text2);
					scene->addItem(some_text3);
					scene->addItem(some_text4);
				}
				flag = !flag;
		} );
	}
}

void View::draw_map() {
    QBrush brush;
    brush.setTextureImage(QImage("data/map.jpg"));

    Button *map = new Button();
    map->setRect(0, 0, 1320, 880);
    map->set_clickable(false);
    map->setBrush(brush);
	map->setZValue(map_z_value);
	// TODO
    scene->addItem(map);
}

void View::create_wagon(const WagonBlock &wagon, int owner) {
    QVector<QPointF> coords;
    for (const auto &point : wagon.coords.points) {
        coords << QPointF(point.x, point.y);
    }

    Wagon *wagon_to_draw = new Wagon(coords, owner != -1? color_frow_owner[owner] : "un_vis");
	wagon_to_draw->setZValue(-1);
	//TODO

	connect(wagon_to_draw, &Wagon::clicked, [=]() {
			Controller->build_path_initialize(wagon.id);
			draw_board();
	} );
	wagon_to_draw->setZValue( owner != -1 ? wagons_visible_z_value : wagons_invisible_z_value);
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

void View::mouseDoubleClickEvent(QMouseEvent *event) {
	int x = event->pos().x();
	int y = event->pos().y();
	const auto &stations = Controller->get_stations();
	for(auto station: stations) {
		const auto &s = station.second;
		const auto &name = station.first;
		int cx = s.p.x;
		int cy = s.p.y;
		int r = 20;

		if(r * r > (cx - x) * (cx - x) + (cy - y) * (cy - y)){
			std::cout << name << '\n';
			Controller->build_station(name);
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

	deck->setZValue(deck_z_vzlue);

    scene->addItem(deck);
}

void View::get_card_from_deck() {
	Controller->get_card_from_deck();
    draw_board();

	/*
	// Wagon *w = new Wagon(coords, card.color);
	Wagon *w = new Wagon(coords, card.color);
	const auto &color = card.color;
	int index = color_to_sdvig[card.color];
	// w->setZValue(animation_z_value);
	w->setZValue(-1);
	auto *wid = qobject_cast<QWidget *>(w);
	wid->setAutoFillBackground(true);
	QPalette Pal(palette());

	if (color == White) {
		Pal.setColor(QPalette::Background, Qt::white);
	} else if (color == Orange) {
		Pal.setColor(QPalette::Background, QColor("orange"));
	} else if (color == Green) {
		Pal.setColor(QPalette::Background, Qt::green);
	} else if (color == Red) {
		Pal.setColor(QPalette::Background, Qt::red);
	} else if (color == Black) {
		Pal.setColor(QPalette::Background, Qt::black);
	} else if (color == Blue) {
		Pal.setColor(QPalette::Background, Qt::blue);
	} else if (color == Yellow) {
		Pal.setColor(QPalette::Background, Qt::yellow);
	} else if (color == Purple) {
		Pal.setColor(QPalette::Background, Qt::magenta);
	} else if (color == Uncolored || color == Multicolored) {
		Pal.setColor(QPalette::Background, Qt::gray);
	} else {
		std::cout << color << '\n';
	}

	wid->setPalette(Pal);
	QPropertyAnimation *animation = new QPropertyAnimation(w, "geometry");
	animation->setDuration(100);

	animation->setKeyValueAt(0, 
	QRect(1920 - width, height * (i), width, height));
	animation->setKeyValueAt(1, 
			QRect(177 * color_to_sdvig[card.color], 1080 - 150, 177, 150));

	scene->addWidget(w);
	// animation->start(QAbstractAnimation::DeleteWhenStopped);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
	*/
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
				draw_board();
		} );
		wagon_to_draw->setZValue(players_cards_z_value);
        scene->addItem(wagon_to_draw);

		QGraphicsTextItem* cur_color_count = new QGraphicsTextItem(QString::number(count[card.color]));
		QFont font("comic sans",50);
		cur_color_count->setFont(font);
		cur_color_count->setPos(width * (color_to_sdvig[card.color] + 0.5), 1080 - height);
		scene->addItem(cur_color_count);
    }
}

namespace {
	void delay(int seconds) {
	std::cout << "delay(" << seconds << ")\n";
    time_t start, current;

    time(&start);

    do
    {
        time(&current);
    }
    while ((current - start) < seconds);
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
				draw_board();

				// Wagon *w = new Wagon(coords, card.color);
				Wagon *w = new Wagon(coords, card.color);
				const auto &color = card.color;
				int index = color_to_sdvig[card.color];
				// w->setZValue(animation_z_value);
				w->setZValue(-1);
				auto *wid = qobject_cast<QWidget *>(w);
				wid->setAutoFillBackground(true);
				QPalette Pal(palette());

				if (color == White) {
					Pal.setColor(QPalette::Window, Qt::white);
				} else if (color == Orange) {
					Pal.setColor(QPalette::Window, QColor("orange"));
				} else if (color == Green) {
					Pal.setColor(QPalette::Window, Qt::green);
				} else if (color == Red) {
					Pal.setColor(QPalette::Window, Qt::red);
				} else if (color == Black) {
					Pal.setColor(QPalette::Window, Qt::black);
				} else if (color == Blue) {
					Pal.setColor(QPalette::Window, Qt::blue);
				} else if (color == Yellow) {
					Pal.setColor(QPalette::Window, Qt::yellow);
				} else if (color == Purple) {
					Pal.setColor(QPalette::Window, Qt::magenta);
				} else if (color == Uncolored || color == Multicolored) {
					Pal.setColor(QPalette::Window, Qt::gray);
				} else {
					std::cout << color << '\n';
				}

				wid->setPalette(Pal);
				QPropertyAnimation *animation = new QPropertyAnimation(w, "geometry");
				animation->setDuration(100);

				animation->setKeyValueAt(0, 
				QRect(1920 - width, height * (i), width, height));
				/*
				animation->setKeyValueAt(0, 
				QRect(0, 0, 200, 200));
				*/
				animation->setKeyValueAt(1, 
						QRect(177 * color_to_sdvig[card.color], 1080 - 150, 177, 150));
				scene->addWidget(w);
				connect(animation, &QPropertyAnimation::finished, [&](){
						// scene->removeItem(w->graphicsProxyWidget());
					});
				// animation->start(QAbstractAnimation::DeleteWhenStopped);
				animation->start();
		} );
		wagon_to_draw->setZValue(active_card_z_value);
		scene->addItem(wagon_to_draw);
	}
}

void View::end_game() {
	int height = 30;
	const auto &results = Controller->get_results();
	for(int i = 0; i < results.size(); ++ i) {
		QGraphicsTextItem* some_text = new QGraphicsTextItem(QString("Player ") 
				+ QString::number(i) 
				+ QString(" have: ") 
				+ QString::number(results[i]));
		some_text->setFont(QFont("comic sans", 14));
		some_text->setPos(0, height * i);
		scene->addItem(some_text);
	}
}
