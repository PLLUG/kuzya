#ifndef WTACHWINDOW_H
#define WTACHWINDOW_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QToolBar>

#include "variable.h"
#include "gdb.h"


struct VarComp {
    bool operator()(const Variable& a, const Variable& b) const {
        return a.getName() < b.getName();
    }
};

class WatchWindow : public QWidget
{
    Q_OBJECT
public:
    WatchWindow(QWidget *parent);
    void addTreeRoot(Variable var);
    void addTreeChild(QTreeWidgetItem *parent,
                      Variable var, QString prefix, bool internal);
    void addTreeChildren(QTreeWidgetItem* parrent,
                      Variable var, QString prefix, bool drfPointer = false);

    void moidifyTreeItemPointer(QTreeWidgetItem* itemPointer);
    void setDebugger(Gdb* debugger);
    QToolBar* debugButtons;
private:
    Gdb* mGdbDebugger;
    QTreeWidget* mWatchLocalsWidget;
    std::list<QTreeWidgetItem*> mPointers;
    std::map<QTreeWidgetItem*, Variable> mPointersName;
    std::map<Variable, QTreeWidgetItem*, VarComp> mTypeVar;
    std::map<Variable, QTreeWidgetItem*, VarComp> mPointersContent;
};

#endif // WTACHWINDOW_H
