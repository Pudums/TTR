#include "Wagon.h"
#include <QBrush>

Wagon::Wagon(const QVector<QPointF> &points, const std::string &color) {
	QPolygonF wagon_polygon(points);
	setPolygon(wagon_polygon);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	if(color == "White") {
		brush.setColor(Qt::white);
	} else if (color == "Orange") {
		brush.setColor(QColor("orange"));
	} else if (color == "Green") {
		brush.setColor(Qt::green);
	} else if (color == "Red") {
		brush.setColor(Qt::red);
	} else if (color == "Black") {
		brush.setColor(Qt::black);
	} else if (color == "Blue") {
		brush.setColor(Qt::blue);
	} else if (color == "Yellow") {
		brush.setColor(Qt::yellow);
	} else if (color == "Purple") {
		brush.setColor(Qt::magenta);
	} else if (color == "Uncolored") {
		brush.setColor(Qt::gray);
	}

	setBrush(brush);
}
