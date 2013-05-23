#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
class Bone;

class Edge : public QGraphicsItem
{
public:
    Edge(Bone* sourceBone, Bone* destBone);
    ~Edge();

    void setSourceBone(Bone* s);
    Bone* sourceBone()const;

    void setDestBone(Bone* d);
    Bone* destBone()const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void adjust();

private:
    Bone*   _sourceBone;
    Bone*   _destBone;
    QPointF _sourcePoint;
    QPointF _destPoint;
};

#endif // EDGE_H
