#include "watchwindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

WatchWindow::WatchWindow(QWidget* parent):
    QWidget(parent)
{
        this->setAutoFillBackground(true);
        QVBoxLayout *innerLabelLayout = new QVBoxLayout(this);
        this->setLayout(innerLabelLayout);
        debugButtons = new QToolBar(this);
        mWatchLocalsWidget = new QTreeWidget(this);
        mWatchLocalsWidget->setColumnCount(3);
        connect(mWatchLocalsWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotExpandVariable(QTreeWidgetItem*,int)), Qt::UniqueConnection);
        connect(mWatchLocalsWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(slotItemVariableExpanded(QTreeWidgetItem*)), Qt::UniqueConnection);
        mWatchLocalsWidget->setStyleSheet(
                    "QTreeView::branch:!has-children:!has-siblings:adjoins-item,"
                    "QTreeView::branch:has-siblings:adjoins-item,"
                    "QTreeView::branch:has-siblings:!adjoins-item {"
                        "border-image: url(:/treeView/empty);"
                    "}"
                    "QTreeView::branch:open:has-children:!has-siblings,"
                    "QTreeView::branch:open:has-children:has-siblings  {"
                            "border-image: none;"
                            "image: url(:/treeView/expand);"
                    "}"
                    "QTreeView::branch:has-children:!has-siblings:closed,"
                    "QTreeView::branch:closed:has-children:has-siblings {"
                            "border-image: none;"
                            "image: url(:/treeView/hide);"
                    "}"
                    "QTreeView::item:hover{background-color:#EEFFFF;}"
                                          );
        mWatchLocalsWidget->setSelectionMode(QAbstractItemView::NoSelection);
        mWatchLocalsWidget->setFocusPolicy(Qt::NoFocus);
        mWatchLocalsWidget->setColumnCount(3);
        mWatchLocalsWidget->setHeaderLabels(QStringList() << "Name" << "Content" << "Type");
        qDebug() << mWatchLocalsWidget->styleSheet();
        innerLabelLayout->addWidget(debugButtons);
        innerLabelLayout->addWidget(mWatchLocalsWidget);

        debugButtons->setIconSize(QSize(30,15));

        //mWatchLocalsWidget
//        addTreeRoot(Variable("a", "int", "3"));

}

void WatchWindow::addTreeRoot(Variable var)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(mWatchLocalsWidget);
    mTypeVar[var] = treeItem;
    mGdbDebugger->getVarType(var);
    treeItem->setText(0, var.getName());
    QString varContent = var.getContent();
    treeItem->setText(1, varContent);
    addTreeChildren(treeItem, var, "");
}

void WatchWindow::addTreeChild(QTreeWidgetItem *parent, Variable var, QString prefix, bool internal = false)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    mTypeVar[var] = treeItem;
    if(!internal)
    {
        mGdbDebugger->getVarType(var);
    }


    QString plainName = var.getName().split('.').last();
    treeItem->setText(0, plainName);
    treeItem->setText(1, var.getContent());
    treeItem->setText(2, var.getType());
    if(!internal)
    {
        addTreeChildren(treeItem, var, prefix);
    }
    else
    {
        treeItem->setHidden(true);
    }
    parent->addChild(treeItem);
}

void WatchWindow::addTreeChildren(QTreeWidgetItem *parrent, Variable var, QString prefix, bool drfPointer)
{
    std::vector<Variable> nestedTypes = var.getNestedTypes();
    if(drfPointer && nestedTypes.size() ==0)
    {
        addTreeChild(parrent, var, "", false);
    }
    else if(var.isPointer())
    {
                QString dereferencedVarName = QString("*(%1)").arg(var.getName());
        addTreeChild(parrent, var, "", true);   //create fake node to enable expanding parent
        mPointersName[parrent] = var;   //Add pointer's node to map and attach to this node pointer
    }
    for(auto i : nestedTypes)
    {
    //        throw;
    //        QString likelyType;/* = mGdbDebugger->getVarType(i.getName());*/
    //        i.setType(likelyType.isEmpty() ? "<No info>" : likelyType);
        addTreeChild(parrent, i, prefix, false);
    }
}

void WatchWindow::moidifyTreeItemPointer(QTreeWidgetItem *itemPointer)
{   //remove helper node and attachs content of dereferenced pointer
    Variable pointer = mPointersName[itemPointer];
    QString drfName = tr("(*%1)").arg(pointer.getName());
    Variable drfPointer(drfName, "", "");
    mPointersContent[drfPointer] = itemPointer;
    mGdbDebugger->getVarContent(drfName);
    QTreeWidgetItem* child = itemPointer->child(0); //Pointer's node always has ony one shils so it's index is '0'
    itemPointer->removeChild(child);    //remove internal node in tree
}

void WatchWindow::setDebugger(Gdb *debugger)
{
    mGdbDebugger = debugger;
}
