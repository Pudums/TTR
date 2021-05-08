#ifndef __View__H__
#define __View__H__

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <vector>
#include <memory>
#include "Wagon.h"
#include "TTRController.h"
#include "Server/TTRServer.h"

class View : public QGraphicsView {
    Q_OBJECT
    TTRController *Controller;
    ttr::LocalServer* server;

    void create_count_of_players();
    void draw_deck();
    void draw_wagons();
    void draw_map();
	void draw_wagons_count();
    void create_wagon(const WagonBlock &wagon, int owner);
	void draw_active_cards();
    void draw_players_cards();
	void end_game();
	void draw_stations();			// TODO
	void choose_count_of_bots(int count, bool is_server = true);
	void mouseDoubleClickEvent(QMouseEvent *event);

public:
    void draw_board();
    View(QWidget *parrent = nullptr);

    QGraphicsScene *scene;

    void display_menu();

public slots:
    void start(bool is_server = true);
    void start_player_1(bool is_server = true);
    void start_player_2(bool is_server = true);
    void start_player_3(bool is_server = true);
    void start_player_4(bool is_server = true);
    void get_card_from_deck();
};

#endif  //__View__H__
