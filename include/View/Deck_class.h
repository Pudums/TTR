#ifndef DECK__H__
#define DECK__H__

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsWidget>
#include <QWidget>
#include <string>

class Deck_class : public QWidget {
    // Q_OBJECT
public:
    Deck_class();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();
};

#endif  // DECK__H__
