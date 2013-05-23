#include "bone.h"
#include <QPainter>
#include <QDebug>
#include "edge.h"

Edge::Edge(Bone* sourceBone, Bone* destBone)
    :_sourceBone(sourceBone), _destBone(destBone)
{
    _sourceBone->addEdge(this);
    //_destBone->addEdge(this);
}

Edge::~Edge()
{

}

void Edge::setSourceBone(Bone* s)
{
    _sourceBone = s;
    adjust();
}

Bone* Edge::sourceBone()const
{
    return _sourceBone;
}

void Edge::setDestBone(Bone* d)
{
    _destBone = d;
    adjust();
}

Bone* Edge::destBone()const
{
    return _destBone;
}

void Edge::adjust()
{
    if(_sourceBone == NULL  ||  _destBone == NULL)
        return;
    QLineF line(mapFromItem(_sourceBone,0,0), mapFromItem(_destBone, 0, 0));
    qreal length = line.length();
    prepareGeometryChange();

    if(length > qreal(10.0))
    {
        QPoint edgeOffset((line.dx()*5)/length, (line.dy()*5)/length);
        _sourcePoint = line.p1() + edgeOffset;
        _destPoint = line.p2() - edgeOffset;
    }
    else
    {
        _sourcePoint = _destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if(!_sourceBone || !_destBone)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth) / 2.0;

    return QRectF(_sourcePoint, QSizeF(_destPoint.x() - _sourcePoint.x(),
                                      _destPoint.y() - _sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                 QWidget *)
{
    if(!_sourceBone || !_destBone)
        return;

    QLineF line(_sourcePoint, _destPoint);

    if (qFuzzyCompare(line.length(), qreal(0.0)))
        return;

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}
