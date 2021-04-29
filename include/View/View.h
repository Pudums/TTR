#ifndef __View__H__
#define __View__H__

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <vector>
#include <memory>
#include "Wagon.h"
#include "TTRController.h"

class View : public QGraphicsView {
    Q_OBJECT
    TTRController *Controller;
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
	void while_true();				// TODO
	void choose_count_of_bots(int count);
	std::vector<Wagon *> to_draw;	// TODO
//	void mousePressEvent(QMouseEvent *event);

public:
    void draw_board();
    View(QWidget *parrent = nullptr);

    QGraphicsScene *scene;

    void display_menu();

    // signals:
public slots:
    void start();
    void start_player_1();
    void start_player_2();
    void start_player_3();
    void start_player_4();
    void get_card_from_deck();
};

#endif  //__View__H__
