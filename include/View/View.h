#ifndef __View__H__
#define __View__H__

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include "TTRController.h"

class View : public QGraphicsView {
    Q_OBJECT
    TTRController *Controller;
    void create_count_of_players();
    void draw_board();
    void draw_deck();
    void draw_wagons();
    void draw_map();
    void create_wagon(const WagonBlock &wagon, bool is_visible);
    void draw_players_cards();

public:
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
