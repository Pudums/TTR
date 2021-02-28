#ifndef __WAGON_H__
#define __WAGON_H__

#include <QGraphicsPolygonItem>

class Wagon : public QGraphicsPolygonItem {
public:
	Wagon(QVector<QPointF> &points);
};

#endif // __WAGON_H__
