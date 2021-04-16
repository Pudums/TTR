#include "Wagon.h"
#include <QBrush>
#include <iostream>
#include "WagonCard.h"

Wagon::Wagon(const QVector<QPointF> &points, const std::string &color) {
    QPolygonF wagon_polygon(points);
    setPolygon(wagon_polygon);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::cyan);
    bool flag = true;
    if (color == White) {
        brush.setColor(Qt::white);
    } else if (color == Orange) {
        brush.setColor(QColor("orange"));
    } else if (color == Green) {
        brush.setColor(Qt::green);
    } else if (color == Red) {
        brush.setColor(Qt::red);
    } else if (color == Black) {
        brush.setColor(Qt::black);
    } else if (color == Blue) {
        brush.setColor(Qt::blue);
    } else if (color == Yellow) {
        brush.setColor(Qt::yellow);
    } else if (color == Purple) {
        brush.setColor(Qt::magenta);
    } else if (color == Uncolored || color == Multicolored) {
        brush.setColor(Qt::gray);
    } else if (color == "un_vis") {
        flag = false;
    } else {
        std::cout << color << '\n';
    }

    if (flag)
        setBrush(brush);
}

void Wagon::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

void Wagon::make_step() {
}
