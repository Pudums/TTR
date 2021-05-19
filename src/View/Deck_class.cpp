#include "Deck_class.h"

Deck_class::Deck_class() {
}

void Deck_class::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}
