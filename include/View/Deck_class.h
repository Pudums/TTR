#ifndef DECK__H__
#define DECK__H__

#include <QGraphicsWidget>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QWidget>
#include <string>

class Deck_class: public QWidget {
	// Q_OBJECT
public:
    Deck_class();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
	void clicked();
};

#endif  // DECK__H__
