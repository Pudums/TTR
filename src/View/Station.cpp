#include "Station.h"

void Station::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}
