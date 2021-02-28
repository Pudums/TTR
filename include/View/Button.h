#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Button : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    // constructors
    Button(QString name, QGraphicsItem *parent = NULL);

    // public methods (events)
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void clicked();

private:
    QGraphicsTextItem *text;
};

#endif  // __BUTTON_H__
