#include "Button.h"
#include <QBrush>
#include <QGraphicsTextItem>

Button::Button(QGraphicsItem *parent)
    : QGraphicsRectItem(parent) {
    setAcceptHoverEvents(true);
	image = true;
}

Button::Button(QString name, QGraphicsItem *parent)
    : QGraphicsRectItem(parent) {
    setRect(0, 0, 200, 50);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkBlue);
    setBrush(brush);

    text = new QGraphicsTextItem(name, this);
    int xPos = rect().width() / 2 - text->boundingRect().width() / 2;
    int yPos = rect().height() / 2 - text->boundingRect().height() / 2;
    text->setPos(xPos, yPos);

    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if(clickable)
		emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
	if(!image) {
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(Qt::blue);
		setBrush(brush);
	}
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
	if(!image) {
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(Qt::darkBlue);
		setBrush(brush);
	}
}
