#include "Wagon.h"
#include <QBrush>

Wagon::Wagon(QVector<QPointF> &points) {
	QPolygonF wagon_polygon(points);
	setPolygon(wagon_polygon);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(Qt::lightGray);
	setBrush(brush);
}
