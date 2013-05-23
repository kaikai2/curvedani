#include <QGraphicsScene>
#include <QDockWidget>
#include <QTreeWidget>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QMenu>
#include <QInputDialog>
#include <QContextMenuEvent>
#include "spinslider.h"
#include "bonestree.h"
#include "motiontree.h"
#include "editor.h"
#include "bone.h"
#include "edge.h"

Editor::Editor(QWidget *parent) :
    QGraphicsView(parent)
{
    createBoneMenu();
    createDockWidgets();
    _scene = new QGraphicsScene(this);
    _scene->setSceneRect(-500, -500, 1000, 1000);
    setScene(_scene);

    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(ScrollHandDrag);

    OriginBone* originBone = new OriginBone(this);
    scene()->addItem(originBone);
    centerOn(originBone); // view center

    Bone* bone = new Bone(this, originBone);
}

void Editor::createDockWidgets()
{
    _zoomSlider = new SpinSlider("Zoom", "linear", "real", 0.10, 2.00, this);
    connect(_zoomSlider, SIGNAL(valueChanged(qreal)), this, SLOT(zoomValueChanged(qreal)));
    _zoomSlider->setValue(1.0);

    _optionDock = new QDockWidget(tr("Option"), this);
    _optionDock->setWidget(_zoomSlider);

    _bonesTree = new BonesTree(this);
    connect(_bonesTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(selectedViewBone(QTreeWidgetItem*)));
    _bonesDock = new QDockWidget(tr("Bones Manager"), this);
    _bonesDock->setWidget(_bonesTree);

    _motionTree = new MotionTree(this);
    _motionDock = new QDockWidget(tr("Motion Manager"), this);
    _motionDock->setWidget(_motionTree);
}

void Editor::zoomValueChanged(qreal value)
{
    QMatrix oldMatrix = matrix();
    resetMatrix();
    translate(oldMatrix.dx(), oldMatrix.dy());
    scale(value, value);
}

QDockWidget* Editor::optionDock() const
{
    return _optionDock;
}

QDockWidget* Editor::bonesDock() const
{
    return _bonesDock;
}

QDockWidget* Editor::motionDock() const
{
    return _motionDock;
}

/*
void Editor::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug() << "Editor::contextMenuEvent" << event->globalPos();
    event->ignore();
}
*/

void Editor::createBoneMenu()
{   
    QAction* newBoneAct = new QAction(tr("New Bone"), this);
    connect(newBoneAct, SIGNAL(triggered()), this, SLOT(newBone()));

    QAction* delBoneAct = new QAction(tr("Delete Bone"), this);
    connect(delBoneAct, SIGNAL(triggered()), this, SLOT(deleteBone()));

    QAction* renameBoneAct = new QAction(tr("Rename Bone"), this);
    connect(renameBoneAct, SIGNAL(triggered()), this, SLOT(renameBone()));

    _boneMenu = new QMenu(this);
    _boneMenu->addAction(newBoneAct);
    _boneMenu->addAction(delBoneAct);
    _boneMenu->addAction(renameBoneAct);
}

QMenu* Editor::boneContextMenu()const
{
    return _boneMenu;
}

void Editor::newBone()
{
    foreach (QGraphicsItem *item, scene()->selectedItems())
    {
        Bone* boneParent = qgraphicsitem_cast<Bone*>(item);
        Bone* bone = new Bone(this, boneParent);
        _bonesTree->newBone(bone);
        connect(bone, SIGNAL(boneChanged(Bone*)), _bonesTree, SLOT(updateBone(Bone*)));
        connect(bone, SIGNAL(boneRemoved(Bone*)), _bonesTree, SLOT(removeBone(Bone*)));
    }
}

void Editor::deleteBone()
{
    foreach (QGraphicsItem *item, scene()->selectedItems())
    {
        Bone* bone = qgraphicsitem_cast<Bone*>(item);
        bone->removeEdges();
        bone->childAlsoRemoved();
        scene()->removeItem(bone);
    }
}

void Editor::renameBone()
{
    foreach (QGraphicsItem *item, scene()->selectedItems())
    {
        Bone* bone = qgraphicsitem_cast<Bone*>(item);
        if(bone)
        {
            bool ok;
            QString newName = QInputDialog::getText(this, tr("Rename bone"), tr("New name"),
                                                    QLineEdit::Normal, bone->name(), &ok);
            if(ok)
            {
                bone->setName(newName);
                _bonesTree->updateBone(bone);
            }
        }
    }
}

void Editor::selectedTreeBone(QGraphicsItem* itemG)
{
    _bonesTree->clearSelection();
    for(int i = 0; i<_bonesTree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* itemT = _bonesTree->topLevelItem(i);
        if(itemG == qVariantValue<QGraphicsItem*>(itemT->data(0,Qt::UserRole)))
        {
            itemT->setSelected(true);
        }
    }
}

void Editor::selectedViewBone(QTreeWidgetItem* itemT)
{
    scene()->clearSelection();
    if(itemT->type() != BonesTree::BoneType)
        itemT = itemT->parent();
    QGraphicsItem* itemG = qVariantValue<QGraphicsItem*>(itemT->data(0,Qt::UserRole));
    if(itemG)
    {
        itemG->setSelected(true);
    }
}

