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
};

#endif  //__View__H__
