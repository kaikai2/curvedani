#ifndef BONESTREE_H
#define BONESTREE_H

#include <QTreeWidget>
#include <QHash>
class QTreeWidgetItem;
class Bone;

class BonesTree : public QTreeWidget
{
Q_OBJECT
public:
    explicit BonesTree(QWidget *parent = 0);
    enum {BoneType, TitleType, PositionType, ImageType};
    QTreeWidgetItem* newBone(Bone* bone);

public slots:
    void updateBone(Bone* bone);
    void removeBone(Bone* bone);

private:    
    void addTestElement();
    QHash<Bone*, QTreeWidgetItem*> _boneToItemHash;

};

#endif // BONESTREE_H
