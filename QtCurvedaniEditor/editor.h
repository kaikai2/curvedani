#ifndef EDITOR_H
#define EDITOR_H

#include <QGraphicsView>

class QGraphicsScene;
class QDockWidget;
class SpinSlider;
class QTreeWidget;
class QTreeWidgetItem;
class BonesTree;
class MotionTree;
class QMenu;

class Editor : public QGraphicsView
{
Q_OBJECT
public:
    explicit Editor(QWidget *parent = 0);

    QDockWidget* optionDock() const;
    QDockWidget* bonesDock() const;
    QDockWidget* motionDock() const;
    QMenu*       boneContextMenu()const;

public slots:
    void selectedTreeBone(QGraphicsItem* itemG);
    void selectedViewBone(QTreeWidgetItem* itemT);

protected slots:
    void zoomValueChanged(qreal value);
    void newBone();
    void deleteBone();
    void renameBone();

private:
    QMenu*              _boneMenu;
    QGraphicsScene*     _scene;
    QDockWidget*        _optionDock;
    QDockWidget*        _bonesDock;
    QDockWidget*        _motionDock;
    SpinSlider*         _zoomSlider;
    BonesTree*          _bonesTree;
    MotionTree*         _motionTree;

    void createDockWidgets();
    void createBoneMenu();
};

#endif // EDITOR_H
