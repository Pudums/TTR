#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Button : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    // constructors
    Button(QString name, QGraphicsItem *parent = NULL);
    Button(QGraphicsItem *parent = NULL);

    // public methods (events)
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void set_clickable(bool clickable_) {
        clickable = clickable_;
    }
signals:
    void clicked();

private:
    QGraphicsTextItem *text;
    bool image = false;
    bool clickable = true;
};

#endif  // __BUTTON_H__
