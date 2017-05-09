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
        mDebugButtons = new QToolBar(this);
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
        innerLabelLayout->addWidget(mDebugButtons);
        innerLabelLayout->addWidget(mWatchLocalsWidget);

        mDebugButtons->setIconSize(QSize(30,15));

        connect(mWatchLocalsWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this,
                SLOT(slotPointerItemExpanded(QTreeWidgetItem*)), Qt::UniqueConnection);
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
    connect(mGdbDebugger, SIGNAL(signalUpdatedVariables()), this, SLOT(slotShowVariables()), Qt::UniqueConnection);
    connect(mGdbDebugger, SIGNAL(signalTypeUpdated(Variable)), this, SLOT(slotTypeUpdated(Variable)), Qt::UniqueConnection);
    connect(mGdbDebugger, SIGNAL(signalContentUpdated(Variable)), this, SLOT(slotDereferenceVar(Variable)), Qt::UniqueConnection);
    connect(mGdbDebugger, SIGNAL(signalBreakpointHit(int)), this, SLOT(slotBreakpointHit(int)), Qt::UniqueConnection);
}

void WatchWindow::slotUpdateVariables()
{
    mGdbDebugger->updateVariable64x();
}

QToolBar *WatchWindow::getDebugButtonPanel() const
{
    return mDebugButtons;
}

void WatchWindow::slotShowVariables()
{
    auto locals = mGdbDebugger->getLocalVariables();
    mWatchLocalsWidget->clear();
    for(auto i : locals)
    {
        addTreeRoot(i);
    }
}

void WatchWindow::slotTypeUpdated(Variable var)
{
    auto iterator = (find_if(mPointersContent.begin(), mPointersContent.end(),
                                    [&](std::pair<Variable, QTreeWidgetItem*> item)
                            {
                                return (var.getName() == item.first.getName());
                            }));
    if(iterator != mPointersContent.end())
    {
        QTreeWidgetItem* itemPointer = (find_if(mPointersContent.begin(), mPointersContent.end(),
                                    [&](std::pair<Variable, QTreeWidgetItem*> item)
                            {
                                return (var.getName() == item.first.getName());
                            })->second);
        if(var.isPointer())
        {
            var.setContent(var.getContent().replace(tr("(%1)").arg(var.getType()), ""));
        }
        auto nestedTypes = var.getNestedTypes();
        bool isNotPointer = !var.isPointer();
        if(nestedTypes.size() == 0 && isNotPointer)
        {
                addTreeChild(itemPointer, var, "", false);
                mPointersContent.erase(iterator);
                return;
        }
        addTreeChildren(itemPointer, var, "", true);   //append dereferenced pointer to node with addres
        mPointersContent.erase(iterator);
    }
    else
    {
        QTreeWidgetItem* item = mTypeVar[var];
        item->setText(2, var.getType());
        if(var.isPointer())
        {
            QString dereferencedVarName = QString("*(%1)").arg(var.getName());
            addTreeChild(item, var, "", true);   //create fake node to enable expanding parent
            mPointersName[item] = var;   //Add pointer's node to map and attach to this node pointer
        }
    }

}

void WatchWindow::slotDereferenceVar(Variable var)
{
    mGdbDebugger->getVarType(var);
}

void WatchWindow::slotBreakpointHit(int line)
{
//    QMessageBox msg;
//    msg.setText(tr("Programm hit breakpoint at %1 line").arg(QString::number(line)));
//    msg.exec();
    slotUpdateLocals();
}

void WatchWindow::slotUpdateLocals()
{
    mGdbDebugger->updateVariable64x();
}

void WatchWindow::slotPointerItemExpanded(QTreeWidgetItem *item)
{
    auto foundIterator = mPointersName.find(item);
    if(foundIterator != mPointersName.end())
    {
            moidifyTreeItemPointer(item);
            mPointersName.erase(foundIterator);
    }
}
