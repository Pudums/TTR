#ifndef __View__H__
#define __View__H__

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

class View: public QGraphicsView {
	Q_OBJECT
public:
	View(QWidget *parrent = nullptr);

	QGraphicsScene* scene;

	void display_menu();

//signals:
public slots:
	void start();
};

#endif //__View__H__
