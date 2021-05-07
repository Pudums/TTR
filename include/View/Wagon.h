#ifndef __WAGON_H__
#define __WAGON_H__

#include <QGraphicsWidget>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QWidget>
#include <string>

class Wagon : public QWidget, public QGraphicsPolygonItem {
	Q_OBJECT
	// Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Wagon(const QVector<QPointF> &points, const std::string &color);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

	// QPointF pos() const;
	// void setPos(QPointF& pos);
	void make_step();
signals:
	void clicked();
};

#endif  // __WAGON_H__
