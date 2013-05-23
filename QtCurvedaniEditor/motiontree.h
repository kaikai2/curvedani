#ifndef MOTIONTREE_H
#define MOTIONTREE_H

#include <QTreeWidget>
class QTreeWidgetItem;

class MotionTree : public QTreeWidget
{
Q_OBJECT
public:
    explicit MotionTree(QWidget *parent = 0);
    enum {Frame, Motion};

public slots:
    QTreeWidgetItem* newMotion(const QString& motionName);
    QTreeWidgetItem* newFrame(QTreeWidgetItem* motion, const QString& frameName);

private:
    void addTestElement();


};

#endif // MOTIONTREE_H
