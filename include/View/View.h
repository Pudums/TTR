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
public:
    View(QWidget *parrent = nullptr, TTRController *contr = nullptr);

    QGraphicsScene *scene;

    void display_menu();

    // signals:
public slots:
    void start();
	void start_player(int count_of_players);
};

#endif  //__View__H__
