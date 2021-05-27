#include "View.h"
#include <QInputDialog>
#include <sstream>
#include <QApplication>
#include <QBrush>
#include <QDesktopWidget>
#include <QGraphicsItemAnimation>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QPalette>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QTextObject>
#include <QTimeLine>
#include <QTimer>
#include <iostream>
#include <string>
#include "Button.h"
#include "CircleWidget.h"
#include "Deck_class.h"
#include "Station.h"
#include "Wagon.h"
#include "WagonCard.h"

namespace {
std::map<std::string, int> color_to_sdvig = {
    {White, 0}, {Orange, 1}, {Green, 2},  {Red, 3},       {Black, 4},
    {Blue, 5},  {Yellow, 6}, {Purple, 7}, {Uncolored, 8}, {Multicolored, 8}};

std::map<int, std::string> color_frow_owner = {{0, Red},
                                               {1, Yellow},
                                               {2, Blue},
                                               {3, Green}};

qreal deck_z_vzlue = 0, map_z_value = 0,
      wagons_invisible_z_value =
          1,  // don't work, cause lay under map and become unclicable
    wagons_visible_z_value = 1, players_cards_z_value = 0,
      active_card_z_value = 0, animation_z_value = -1;
}  // namespace

void View::draw_stations() {
    const auto &stations = Controller->get_stations();
    for (auto station : stations) {
        const auto &s = station.second;
        const auto &name = station.first;

        Station *path = new Station;
        path->addEllipse(QPointF(s.p.x, s.p.y), 20, 20);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
		if(s.r == -1) {
			brush.setColor(Qt::cyan);
		} else {
			auto color = color_frow_owner[s.r];
			if(color == Red) {
				brush.setColor(Qt::red);
			} else if(color ==  Yellow) {
				brush.setColor(Qt::yellow);
			} else if(color == Blue) {
				brush.setColor(Qt::blue);
			} else if(color == Green) {
				brush.setColor(Qt::green);
			}
		}
        QPen *pen = new QPen;
        pen->setBrush(brush);
        connect(path, &Station::clicked,
                [=]() { 
				std::cout << "\t build name = " << name << '\n';
				Controller->build_station(name); });
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
	scene->clear();
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
    // connect(playButton, SIGNAL(clicked()), this, SLOT(start(false)));
    connect(playButton, &Button::clicked, [=]() { host_or_not(false); });
    scene->addItem(playButton);

    Button *playOnlineButton = new Button(QString("Play Online"));
    bxPos = this->width() / 2 - playOnlineButton->boundingRect().width() / 2;
    byPos = 350;
    playOnlineButton->setPos(bxPos, byPos);
    connect(playOnlineButton, &Button::clicked, [=]() { host_or_not(true); });
    scene->addItem(playOnlineButton);

    Button *rules_button = new Button(QString("Rules"));
    int qxPos = this->width() / 2 - rules_button->boundingRect().width() / 2;
    int qyPos = 350 + 125;
    rules_button->setPos(qxPos, qyPos);
    connect(rules_button, SIGNAL(clicked()), this, SLOT(display_rulles()));
    scene->addItem(rules_button);

    Button *quitButton = new Button(QString("Quit"));
    qxPos = this->width() / 2 - quitButton->boundingRect().width() / 2;
    qyPos = 350 + 125 + 125;
    quitButton->setPos(qxPos, qyPos);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    scene->addItem(quitButton);
}

void View::display_rulles() {
	scene->clear();
    Button *back = new Button(QString("Back"));
    int qxPos = 1700;
    int qyPos = 1000;
    back->setPos(qxPos, qyPos);
    connect(back, SIGNAL(clicked()), this, SLOT(display_menu()));
    scene->addItem(back);

	std::stringstream rul;
	{
		std::ifstream in;
		std::string line;
		in.open("data/rules.txt");
		while(!in.eof()) {
			getline(in, line);
			if(line.size() == 0 || line[0] == '#') 
				continue;
			rul << line << '\n';
		}
		in.close();
	}

	std::cout << "rul:\n" << rul.str() << "\nend rul\n";

	//const QString rull = std::string(rul.str());

    QFont font("comic sans", 14);
    QGraphicsTextItem *some_text =
        new QGraphicsTextItem(QString(rul.str().c_str()));
    some_text->setPos(10, 10);
    some_text->setFont(font);
    some_text->setDefaultTextColor(QColor("black"));
    scene->addItem(some_text);
}

void View::start(bool is_server, bool is_host, const char *port) {
    scene->clear();
    std::cout << "start " << is_server << '\n';
    if (is_server && !is_host) {
        /*
        Controller->start_game(-1,
                        -1, type_of_game::LOCAL_CLIENT);
        */
        start_players(-1, is_server, is_host, port);
        return;
    }

    Button *play_1_player_button = new Button(QString("1 Player"));
    int bxPos =
        this->width() / 2 - play_1_player_button->boundingRect().width() / 2;
    int byPos = 150;
    play_1_player_button->setPos(bxPos, byPos);
    connect(play_1_player_button, &Button::clicked,
            [=]() { start_players(1, is_server, is_host, port); });
    scene->addItem(play_1_player_button);

    Button *play_2_player_button = new Button(QString("2 Player"));
    bxPos =
        this->width() / 2 - play_2_player_button->boundingRect().width() / 2;
    byPos = 300;
    play_2_player_button->setPos(bxPos, byPos);
    connect(play_2_player_button, &Button::clicked, [=]() {
        // start_player_2(is_server);
        start_players(2, is_server, is_host, port);
    });
    scene->addItem(play_2_player_button);

    Button *play_3_player_button = new Button(QString("3 Player"));
    bxPos =
        this->width() / 2 - play_3_player_button->boundingRect().width() / 2;
    byPos = 450;
    play_3_player_button->setPos(bxPos, byPos);
    connect(play_3_player_button, &Button::clicked, [=]() {
        start_players(3, is_server, is_host, port);
        // start_player_3(is_server);
    });
    scene->addItem(play_3_player_button);

    Button *play_4_player_button = new Button(QString("4 Player"));
    bxPos =
        this->width() / 2 - play_4_player_button->boundingRect().width() / 2;
    byPos = 600;
    play_4_player_button->setPos(bxPos, byPos);
    connect(play_4_player_button, &Button::clicked, [=]() {
        start_players(4, is_server, is_host, port);
        // start_player_4(is_server);
    });
    scene->addItem(play_4_player_button);
}

void View::start_players(int players, bool is_server, bool is_host, const char *port) {
    std::cout << "start_players " << is_server << '\n';
    choose_count_of_bots(players, is_server, is_host, port);
}

void View::host_or_not(bool is_server) {
    std::cout << "host_or_not "
              << " " << is_server << '\n';
    // choose_count_of_bots(players, is_server);
    if (!is_server) {
        start(is_server, false);
        return;
    }
    scene->clear();
    Button *host = new Button(QString("Host"));
    int bxPos = this->width() / 2 - host->boundingRect().width() / 2;
    int byPos = 150;
    host->setPos(bxPos, byPos);
    connect(host, &Button::clicked, [=]() {
        start(is_server, true);
        // start_players(4, is_server);
        // start_player_4(is_server);
    });
    scene->addItem(host);

    Button *client = new Button(QString("Connect"));
    bxPos = this->width() / 2 - client->boundingRect().width() / 2;
    byPos = 300;
    client->setPos(bxPos, byPos);
    connect(client, &Button::clicked, [=]() {
        std::cout << "cliend clicked "
                  << "\n";
		try {
        start_port(is_server, false);
		} catch (const std::logic_error &e){
		disconnected(e.what());
		}
        // start_players(4, is_server);
        // start_player_4(is_server);
    });
    scene->addItem(client);
}

void View::start_port(bool is_server, bool flag) {
	scene->clear();
	bool ok;
    QString text = QInputDialog::getText(this, tr(""),
                                         tr("Enter ip:"), QLineEdit::Normal,
                                         QString("tmp"), &ok);
	if (ok && !text.isEmpty()) {
		start(is_server, flag, text.toStdString().c_str());
	}
}

void View::timed_redraw() {
    draw_board();
    QTimer *timer = new QTimer();
    timer->setSingleShot(false);
    timer->setInterval(5000);
    connect(timer, &QTimer::timeout, [=]() {
        draw_board();
        // timed_redraw();
        timer->start();
    });
    timer->start();
}

void View::choose_count_of_bots(int n, bool is_server, bool is_host, const char *port) {
    scene->clear();
    std::cout << "choose_count_of_bots " << n << " " << is_server << " "
              << is_host << '\n';
    if (is_server && !is_host) {
        Controller->start_game(-1, -1, type_of_game::LOCAL_CLIENT, port);
        timed_redraw();
        return;
    }

	if(is_server && is_host) {
		Controller->start_game(n, 0, type_of_game::LOCAL_SERVER, port);
		timed_redraw();
		return;
	}

    for (int i = 0; i <= n; i++) {
        Button *bot = new Button(QString::number(i) + QString("Bots"));
        int bxPos = this->width() / 2 - bot->boundingRect().width() / 2;
        int byPos = 150 * (i + 1);
        bot->setPos(bxPos, byPos);
        connect(bot, &Button::clicked, [=]() {
            std::cout << "clicked bots " << n << " " << i << " " << is_server
                      << '\n';
            if (!is_server)
                Controller->start_game(n, i, type_of_game::SINGLE_COMPUTER, port);
            else if (is_host) {
                Controller->start_game(n, i, type_of_game::LOCAL_SERVER, port);
            } else {
                Controller->start_game(n, i, type_of_game::LOCAL_CLIENT, port);
            }
            timed_redraw();
        });
        scene->addItem(bot);
    }
}

void View::draw_board() {
    auto items = scene->items();
    for (auto *i : items) {
        scene->removeItem(i);
        delete i;
    }
    scene->clear();
	try {
    std::cout << "draw_board "
              << "\n";
    std::cout << "start controller.is_game_end\n";
    auto status = Controller->is_game_end();
    std::cout << "end controller.is_game_end\n";
    if (status == 2) {
        std::cout << "status = 2\n";
        Controller->end_game();
        end_game();
        return;
    } else if (status == 1) {
        draw_map();
        draw_wagons();
        draw_wagons_count();
        draw_stations();
    } else {
        std::cout << "draw_deck\n";
        draw_deck();
        std::cout << "draw_map\n";
        draw_map();
        std::cout << "draw_wagons\n";
        draw_wagons();
        std::cout << "draw_players_cards\n";
        draw_players_cards();
        std::cout << "draw_active_cards\n";
        draw_active_cards();
        std::cout << "draw_wagons_count\n";
        draw_wagons_count();
        std::cout << "draw_stations\n";
        draw_stations();
        draw_redraw_button();
		draw_whoose_turn();
    }
	} catch(const std::logic_error &e) {
		std::cout << "server closed\n";
		disconnected(e.what());
	}
}

void View::draw_whoose_turn() {
    QFont font("comic sans", 20);
	font.setBold(true);

    int txPos = 30;
    int tyPos = 30;
	int id = Controller->get_current_player_id();
	int my_id = Controller->get_my_id();
	auto color = color_frow_owner[id];

    QGraphicsTextItem *some_text =
        new QGraphicsTextItem(QString("Curr turn is this color"));
    some_text->setFont(font);
    some_text->setPos(txPos, tyPos);
	some_text->setDefaultTextColor(Qt::cyan);
	if(color == Red) {
		some_text->setDefaultTextColor(Qt::red);
	} else if(color ==  Yellow) {
		some_text->setDefaultTextColor(Qt::yellow);
	} else if(color == Blue) {
		some_text->setDefaultTextColor(Qt::blue);
	} else if(color == Green) {
		some_text->setDefaultTextColor(Qt::green);
	}
    scene->addItem(some_text);

    some_text =
        new QGraphicsTextItem(QString("Ur color is"));
	color = color_frow_owner[my_id];
    some_text->setFont(font);
    some_text->setPos(txPos, tyPos + 22);
	some_text->setDefaultTextColor(Qt::cyan);
	if(color == Red) {
		some_text->setDefaultTextColor(Qt::red);
	} else if(color ==  Yellow) {
		some_text->setDefaultTextColor(Qt::yellow);
	} else if(color == Blue) {
		some_text->setDefaultTextColor(Qt::blue);
	} else if(color == Green) {
		some_text->setDefaultTextColor(Qt::green);
	}
    scene->addItem(some_text);
}

void View::disconnected(const char *err) {
	delete Controller;
	Controller = new TTRController();

    QFont font("comic sans", 14);
    QGraphicsTextItem *some_text =
        new QGraphicsTextItem(QString(err));
    int txPos = this->width() / 2 - some_text->boundingRect().width() / 2;
    int tyPos = 150;
    some_text->setPos(txPos, tyPos);
    some_text->setFont(font);
    some_text->setDefaultTextColor(QColor("black"));
    scene->addItem(some_text);

    Button *main_menu = new Button(QString("Back menu"));
    int bxPos = this->width() / 2 - main_menu->boundingRect().width() / 2;
    int byPos = 300;
    main_menu->setPos(bxPos, byPos);
    connect(main_menu, &Button::clicked, [=]() { display_menu(); });
    scene->addItem(main_menu);
}

void View::draw_redraw_button() {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkRed);
    // brush.setTextureImage(QImage("data/map.jpg"));

    Button *map = new Button();
    map->setRect((1320 / 9) * 9, 880, 279, 200);
    map->setBrush(brush);
    map->setZValue(map_z_value);
	connect(map, &Button::clicked, [&]() {
			Controller->get_routes();
			draw_board();
			});
    // TODO
    scene->addItem(map);

    QFont font("comic sans", 14);
    QGraphicsTextItem *some_text =
        new QGraphicsTextItem(QString("Take new route"));
    some_text->setPos((1320 / 9) * 9 + 20, 880 + 15);
    some_text->setFont(font);
    some_text->setDefaultTextColor(QColor("white"));
    scene->addItem(some_text);
}

namespace {
bool flag = true;
}

void View::draw_wagons_count() {
    const auto &players = Controller->get_players();
    const auto &my_id = Controller->get_my_id();
    int height = 220, width = 367;
    for (int i = 0; i < players.size(); ++i) {
        const auto &player = players[i];
        QFont font("comic sans", 14);

        QVector<QPointF> coords;
        coords << QPointF(1320 + width * 0, height * i)
               << QPointF(1320 + width * 1, height * i)
               << QPointF(1320 + width * 1, height * (i + 1))
               << QPointF(1320 + width * 0, height * (i + 1));

        Wagon *wagon_to_draw = new Wagon(coords, color_frow_owner[i]);
        connect(wagon_to_draw, &Wagon::clicked, [&]() { flag = !flag; draw_board(); });
        scene->addItem(wagon_to_draw);

        if (my_id != i || flag) {
            QGraphicsTextItem *some_text1 = new QGraphicsTextItem(
                QString("Wagons left: ") +
                QString::number(player.number_of_wagons_left));
            some_text1->setFont(font);
            some_text1->setPos(1320 + width * 0.3, i * height);
            scene->addItem(some_text1);

            QGraphicsTextItem *some_text2 = new QGraphicsTextItem(
                QString("Points: ") + QString::number(player.points));
            some_text2->setPos(1320 + width * 0.3, i * height + 30);
            some_text2->setFont(font);
            scene->addItem(some_text2);

            QGraphicsTextItem *some_text3 = new QGraphicsTextItem(
                QString("Stations left: ") +
                QString::number(player.number_of_stations_left));
            some_text3->setPos(1320 + width * 0.3, i * height + 30 * 2);
            some_text3->setFont(font);
            scene->addItem(some_text3);

            QGraphicsTextItem *some_text4 = new QGraphicsTextItem(
                QString("Cards : ") +
                QString::number(player.wagon_cards.size()));
            some_text4->setPos(1320 + width * 0.3, i * height + 30 * 3);
            some_text4->setFont(font);
            scene->addItem(some_text4);

        } else {
            int j = 0;
            for (const auto &path : player.active_routes) {
                QGraphicsTextItem *some_text = new QGraphicsTextItem(
                    QString("From: ") + QString(path.city1.c_str()) +
                    QString(" to ") + QString(path.city2.c_str()));
                some_text->setPos(1320 + width * 0.05,
                                  i * height + 25 * (2 * j));
                some_text->setFont(font);
                scene->addItem(some_text);

                some_text = new QGraphicsTextItem(
                    QString("Left to build: ") +
                    QString::number(path.points_for_passing));
                some_text->setPos(1320 + width * 0.05,
                                  i * height + 25 * (2 * j + 1));
                some_text->setFont(font);
                scene->addItem(some_text);
                j++;
            }
        }
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

    Wagon *wagon_to_draw =
        new Wagon(coords, owner != -1 ? color_frow_owner[owner] : "un_vis");
    wagon_to_draw->setZValue(-1);
    // TODO

    connect(wagon_to_draw, &Wagon::clicked, [=]() {
        Controller->build_path_initialize(wagon.id);
        draw_board();
    });
    wagon_to_draw->setZValue(owner != -1 ? wagons_visible_z_value
                                         : wagons_invisible_z_value);
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
    for (auto station : stations) {
        const auto &s = station.second;
        const auto &name = station.first;
        int cx = s.p.x;
        int cy = s.p.y;
        int r = 20;

        if (r * r > (cx - x) * (cx - x) + (cy - y) * (cy - y)) {
            std::cout << "\t build path = " <<name << '\n';
            Controller->build_station(name);
        }
    }
}

void View::draw_deck() {
    int hight = 200, width = 327;  // free 1593 x 880

    // QBrush q;
    // q.setTextureImage(QImage("data/deck.jpeg"));

    // Button *deck = new Button();

    QVector<QPointF> coords;
    coords << QPointF(1920 - width, 1080 - hight) << QPointF(1920 - width, 1080)
           << QPointF(1920, 1080) << QPointF(1920, 1080 - hight);

    Wagon *wagon_to_draw = new Wagon(coords);

    // Deck_class *deck = new Deck_class();
    QWidget *deck_qw;
    // deck_qw = qobject_cast<QWidget *> (deck);
    deck_qw = new QWidget();
    deck_qw->setStyleSheet("background-image:url(./data/deck.jpg)");
    deck_qw->setAutoFillBackground(true);
    deck_qw->setGeometry(1920 - width, 1080 - hight, width, hight);
    // deck->setBrush(q);
    connect(wagon_to_draw, SIGNAL(clicked()), this, SLOT(get_card_from_deck()));

    // deck->setZValue(deck_z_vzlue);

    scene->addWidget(deck_qw);
    scene->addItem(wagon_to_draw);
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
                    QRect(177 * color_to_sdvig[card.color], 1080 - 150, 177,
    150));

    scene->addWidget(w);
    // animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    */
}

void View::draw_players_cards() {
    std::cout << "start controller.get_current_player_cards\n";
    const auto &cards = Controller->get_current_player_cards();
    std::cout << "end controller.get_current_player_cards\n";
    std::map<std::string, int> count = Controller->get_count_by_color();
    std::cout << "end controller.get_count_by_color\n";
    for (const auto &card : cards) {
        int height = 200, width = 1320 / 9;
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
        });
        wagon_to_draw->setZValue(players_cards_z_value);
        scene->addItem(wagon_to_draw);

        QGraphicsTextItem *cur_color_count =
            new QGraphicsTextItem(QString::number(count[card.color]));
        QFont font("comic sans", 50);
        if (card.color == Black)
            cur_color_count->setDefaultTextColor(QColor("white"));
        cur_color_count->setFont(font);
        cur_color_count->setPos(width * (color_to_sdvig[card.color] + 0.5),
                                1080 - height);
        scene->addItem(cur_color_count);
    }
}

namespace {
void delay(int seconds) {
    std::cout << "delay(" << seconds << ")\n";
    time_t start, current;

    time(&start);

    do {
        time(&current);
    } while ((current - start) < seconds);
}
}  // namespace

void View::draw_active_cards() {
    const auto &cards = Controller->get_active_cards();
    for (int i = 0; i < cards.size(); ++i) {
        const auto &card = cards[i];
        int height = 176, width = 233;
        QVector<QPointF> coords;
        coords << QPointF(1920 - width, height * (i + 1))
               << QPointF(1920 - width, height * (i))
               << QPointF(1920, height * (i))
               << QPointF(1920, height * (i + 1));
        Wagon *wagon_to_draw = new Wagon(coords, card.color);

        connect(wagon_to_draw, &Wagon::clicked, [=]() {
            int height = 200, width = 1320 / 9;
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
            QPropertyAnimation *animation =
                new QPropertyAnimation(w, "geometry");
            animation->setDuration(300);

            animation->setKeyValueAt(
                0, QRect(1920 - width, height * (i), width, height));
            /*
            animation->setKeyValueAt(0,
            QRect(0, 0, 200, 200));
            */
            animation->setKeyValueAt(
                1, QRect(width * color_to_sdvig[card.color] + 1,
                         1080 - height / 2 + 1, width - 2, height - 2));
            scene->addWidget(w);
            connect(animation, &QPropertyAnimation::finished, [&]() {
                // scene->removeItem(w->graphicsProxyWidget());
            });
            // animation->start(QAbstractAnimation::DeleteWhenStopped);
            animation->start();
        });
        wagon_to_draw->setZValue(active_card_z_value);
        scene->addItem(wagon_to_draw);
    }
}

void View::end_game() {
    int height = 30;
    const auto &results = Controller->get_results();
    for (int i = 0; i < results.size(); ++i) {
        QGraphicsTextItem *some_text = new QGraphicsTextItem(
            QString("Player ") + QString::number(i) + QString(" have: ") +
            QString::number(results[i]));
        some_text->setFont(QFont("comic sans", 14));
        some_text->setPos(0, height * i);
        scene->addItem(some_text);
    }
}
