#ifndef BONE_H
#define BONE_H

#include <QGraphicsItem>
class Editor;
class Edge;

class Bone : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Bone(Editor* editor, Bone* parent = NULL);
    ~Bone();

    void addEdge(Edge* edge);
    QList<Edge*> edges()const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updateEdge();
    void removeEdges();

    QString name()const
    {
        return _name;
    }
    void setName(const QString& name = "new Bone")
    {
        _name = name;
        childAlsoChanged();
    }

    QString imagePath()const
    {
        return _imagePath;
    }
    void setImagePath(const QString& imagePath)
    {
        _imagePath = imagePath;
        childAlsoChanged();
    }

    void childAlsoChanged();
    void childAlsoRemoved();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);


private:
    QString         _name;
    QString         _imagePath;
    Editor*         _editor;
    QList<Edge*>    _edgeList;

signals:
    void boneChanged(Bone*);
    void boneRemoved(Bone*);

public slots:
};

class OriginBone: public Bone
{
public:
    OriginBone(Editor* editor, Bone* parent = NULL);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BONE_H
