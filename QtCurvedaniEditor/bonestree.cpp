#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include "bone.h"
#include "bonestree.h"


BonesTree::BonesTree(QWidget *parent) :
    QTreeWidget(parent)
{
    clear();
    QStringList headerLabels;
    headerLabels << tr("Bones") << tr("Attribute");
    this->setHeaderLabels(headerLabels);
}

void BonesTree::addTestElement()
{
}

QTreeWidgetItem* BonesTree::newBone(Bone* bone)
{
    QTreeWidgetItem* boneItem = new QTreeWidgetItem(this, BoneType);
    //boneItem->setData(0, Qt::UserRole, qVariantFromValue(bone));
    boneItem->setText(0,  bone->name());

    QTreeWidgetItem* titleItem = new QTreeWidgetItem(boneItem, TitleType);
    titleItem->setText(0, tr("title"));
    titleItem->setText(1, bone->name());

    QTreeWidgetItem* positionItem = new QTreeWidgetItem(boneItem, PositionType);
    positionItem->setText(0, tr("position"));
    positionItem->setText(1, QString("(%1,%2)").arg(bone->scenePos().x()).arg(bone->scenePos().y()));

    QTreeWidgetItem* imageItem = new QTreeWidgetItem(boneItem, ImageType);
    imageItem->setText(0, tr("image"));
    imageItem->setText(1, bone->imagePath());

    this->addTopLevelItem(boneItem);
    _boneToItemHash.insert(bone, boneItem);
    boneItem->setExpanded(true);
    return boneItem;
}

void BonesTree::updateBone(Bone* bone)
{
    QTreeWidgetItem* boneItem = _boneToItemHash.value(bone, NULL);
    if(boneItem)
    {
        boneItem->setText(0,  bone->name());
        for(int i=0; i<boneItem->childCount(); i++)
        {
            QTreeWidgetItem* item = boneItem->child(i);
            switch(item->type())
            {
            case TitleType:
                item->setText(1, bone->name());
                break;
            case PositionType:
                item->setText(1, QString("(%1,%2)").arg(bone->scenePos().x()).arg(bone->scenePos().y()));
                break;
            case ImageType:
                item->setText(1, bone->imagePath());
                break;
            default:
                break;
            }
        }
        clearSelection();
        boneItem->setSelected(true);
    }
}

void BonesTree::removeBone(Bone *bone)
{
    QTreeWidgetItem* boneItem = _boneToItemHash.value(bone, NULL);
    if(boneItem)
    {
        this->takeTopLevelItem(this->indexOfTopLevelItem(boneItem));
    }
}
