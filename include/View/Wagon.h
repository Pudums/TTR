#ifndef __WAGON_H__
#define __WAGON_H__

#include <QGraphicsPolygonItem>
#include <string>

class Wagon : public QGraphicsPolygonItem {
public:
    Wagon(const QVector<QPointF> &points, const std::string &color);
};

#endif  // __WAGON_H__
