#ifndef __WAGON_H__
#define __WAGON_H__

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <string>

class Wagon :public QObject, public QGraphicsPolygonItem {
	Q_OBJECT
public:
    Wagon(const QVector<QPointF> &points, const std::string &color);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
	void clicked();
};

#endif  // __WAGON_H__
