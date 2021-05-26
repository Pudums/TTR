#ifndef __STATION__H__
#define __STATION__H__

#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>

class Station : public QObject, public QPainterPath {
    Q_OBJECT
public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void clicked();
};

#endif  // __STATION__H__
