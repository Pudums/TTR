#include "Wagon.h"

Wagon::Wagon(QVector<QPointF> &points) {
	QPolygonF wagon_polygon(points);
	setPolygon(wagon_polygon);
}
