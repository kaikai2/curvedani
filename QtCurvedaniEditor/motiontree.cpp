#include <QTreeWidgetItem>
#include "motiontree.h"

MotionTree::MotionTree(QWidget *parent) :
    QTreeWidget(parent)
{
    clear();
    QStringList headerLabels;
    headerLabels << tr("Motion") << tr("Status");
    setHeaderLabels(headerLabels);

    addTestElement();
}

void MotionTree::addTestElement()
{
    QTreeWidgetItem* jumpMotion = newMotion(tr("Jump"));
    newFrame(jumpMotion, "jump1");
    newFrame(jumpMotion, "jump2");
    newFrame(jumpMotion, "jump3");
    newFrame(jumpMotion, "jump4");
    newFrame(jumpMotion, "jump5");

    QTreeWidgetItem* walkMotion = newMotion(tr("Walk"));
    newFrame(walkMotion, "walk1");
    newFrame(walkMotion, "walk2");
    newFrame(walkMotion, "walk3");
}

QTreeWidgetItem* MotionTree::newMotion(const QString& motionName)
{
    QTreeWidgetItem* motionItem = new QTreeWidgetItem(this, Motion);
    motionItem->setText(0, motionName);

    addTopLevelItem(motionItem);
    return motionItem;
}

QTreeWidgetItem* MotionTree::newFrame(QTreeWidgetItem* motion,
                                      const QString& frameName)
{
    QTreeWidgetItem* frameItem = new QTreeWidgetItem(motion, Frame);
    frameItem->setText(0, frameName);

    return frameItem;
}
