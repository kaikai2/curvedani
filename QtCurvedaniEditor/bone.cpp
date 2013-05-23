#include "editor.h"
#include "edge.h"
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include "bone.h"

Bone::Bone(Editor* editor, Bone* parent)
    :QGraphicsItem(parent), _editor(editor)
{
    setName();
    if(parent && _editor)
    {
        int dx = qrand() % 10;
        int dy = 10 - dx;
        switch(qrand()%5)
        {
        case 0:
            moveBy(-10-dx, 10+dy);
            break;
        case 1:
            moveBy(10+dx, -10-dy);
            break;
        case 2:
            moveBy(-10-dx, -10-dy);
            break;
        case 3:
            moveBy(-10-dx, 10+dy);
        default:
            moveBy(10, 10);
            break;
        }

        Edge* edge = new Edge(this, parent);
        _editor->scene()->addItem(edge);
    }

    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Bone::~Bone()
{
    qDebug() << "del bone";
}

QRectF Bone::boundingRect()const
{
    qreal adjust = 2;
    return QRectF(-5 - adjust/2, -5 - adjust/2,
                  10 + adjust, 10 + adjust);
}

void Bone::addEdge(Edge* edge)
{
    _edgeList << edge;
    edge->adjust();
}

QList<Edge*> Bone::edges()const
{
    return _edgeList;
}

void Bone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* )
{
    painter->setBrush(Qt::NoBrush);
    if(isSelected())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::darkGray);
    painter->drawEllipse(-6, -6, 12, 12);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-3, -3, 8, 8);

    QRadialGradient gradient(-3, -3, 8);
    if(option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-5, -5, 10, 10);
}

QPainterPath Bone::shape()const
{
    QPainterPath path;
    path.addEllipse(-5, -5, 10, 10);
    return path;
}

QVariant Bone::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch(change)
    {
    case ItemPositionChange:
        break;
    case ItemPositionHasChanged:
        updateEdge();
        break;
    default:break;
    }
    childAlsoChanged();
    return QGraphicsItem::itemChange(change, value);
}

void Bone::updateEdge()
{
    foreach(Edge *edge, _edgeList)
    {
        if(edge)
            edge->adjust();
    }

    foreach(QGraphicsItem* item, childItems())
    {
        qgraphicsitem_cast<Bone*>(item)->updateEdge();
    }
}

void Bone::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);

    if(isSelected())
    {
        _editor->selectedTreeBone(this);
    }
}

void Bone::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Bone::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(!_editor)
        return;

    scene()->clearSelection();
    setSelected(true);
    if(isSelected())
        _editor->boneContextMenu()->exec(event->screenPos());
}

void Bone::removeEdges()
{
    foreach(Edge* edge, _edgeList)
    {
        scene()->removeItem(edge);
    }
    foreach(QGraphicsItem* item, childItems())
    {
        qgraphicsitem_cast<Bone*>(item)->removeEdges();
    }
}

void Bone::childAlsoChanged()
{
    foreach(QGraphicsItem* item, childItems())
    {
        qgraphicsitem_cast<Bone*>(item)->childAlsoChanged();
    }

    emit boneChanged(this);
}

void Bone::childAlsoRemoved()
{
    foreach(QGraphicsItem* item, childItems())
    {
        qgraphicsitem_cast<Bone*>(item)->childAlsoRemoved();
    }

    emit boneRemoved(this);
}

///////////////////////////////////////////////

OriginBone::OriginBone(Editor *editor, Bone* parent)
    :Bone(editor, parent)
{
    setFlag(ItemIsMovable, false);
    setFlag(ItemIsSelectable, false);
}

void OriginBone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::darkGray);
    painter->setBrush(Qt::gray);
    painter->drawEllipse(-5, -5, 10, 10);
}


