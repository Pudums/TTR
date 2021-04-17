#ifndef __CIRCLEWIDGET__H__
#define __CIRCLEWIDGET__H__

#include <QWidget>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <string>

class CircleWidget : public QWidget{
    Q_OBJECT

public:
    CircleWidget(QWidget *parent = 0);

    void setFloatBased(bool floatBased);
    void setAntialiased(bool antialiased);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void nextAnimationFrame();

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool floatBased;
    bool antialiased;
    int frameNo;
signals:
	void clicked();
};

#endif // __CIRCLEWIDGET__H__
