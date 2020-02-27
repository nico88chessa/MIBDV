#include "MDBreadCrumb.hpp"

#include <QMenu>
#include <QPushButton>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QLabel>

using namespace internal;

const char* MDBreadCrumbPrivate::PATH_DELIMITER = "//";
const char* MDBreadCrumb::DOTS = "...";
const char* MDBreadCrumb::DEFAULT_ROOT_NAME = "Root";

/***********************************************
 *   M D  B R E A D C R U M B  P R I V A T E
 ***********************************************/

MDBreadCrumbPrivate::MDBreadCrumbPrivate(int tailSize) :
    maxTailSize(tailSize), items() {

}

void MDBreadCrumbPrivate::addItem(const QString& key, const QString& value) {

    Item newItem;
    newItem.key = key;
    newItem.value = value;
    items.append(newItem);

}

QString MDBreadCrumbPrivate::getKey(int index) const {

    if (index < size())
        return items.at(index).key;
    return QString();

}

QString MDBreadCrumbPrivate::getValue(int index) const {

    if (index < size())
        return items.at(index).value;
    return QString();

}

void MDBreadCrumbPrivate::removeAll() {
    items.clear();
}

void MDBreadCrumbPrivate::removeItem(int index) {

    if (index == 0)
        return;
    if (index < size()) {
        for (int current = size() - 1; current >= index; --current)
            items.removeAt(current);
    }

}

void MDBreadCrumbPrivate::removeLast() {

    this->removeItem(this->size() - 1);

}

int MDBreadCrumbPrivate::size() const {
    return items.size();
}

bool MDBreadCrumbPrivate::isEmpty() const {
    return size() == 0;
}

bool MDBreadCrumbPrivate::hasCollapsedGroup() const {
    return (items.size() - 1) > maxTailSize;
}

QString MDBreadCrumbPrivate::getRootKey() const {
    return this->getKey(0);
}

QStringList MDBreadCrumbPrivate::getCollapsedGroup() const {

    QStringList temp;
    if (hasCollapsedGroup()) {
        int collapsedGroupSize = size() - 1 - (maxTailSize - 1);
        for (int i=0; i<collapsedGroupSize; ++i)
            temp.append(getKey(i+1));
    }
    return temp;

}

QStringList MDBreadCrumbPrivate::getTail() const {

    QStringList temp;

    int indexStart = hasCollapsedGroup() ? this->size() - (maxTailSize - 1) : 1;
    int count = hasCollapsedGroup() ? maxTailSize - 1 : this->size() - 1;

    for (int c = 0; c < count; ++c ,++indexStart)
        temp.append(getKey(indexStart));

    return temp;
}

int MDBreadCrumbPrivate::getMaxTailSize() const {
    return maxTailSize;
}




/********************************
 *   M D  B R E A D C R U M B
 ********************************/

MDBreadCrumb::MDBreadCrumb(QWidget* parent) :
    dPtr(new internal::MDBreadCrumbPrivate()), QListWidget(parent),
    rootName(DEFAULT_ROOT_NAME), currentPath("") {

    setupUi();
    updateButtonsUi();
    setupSignalsAndSlots();

}

void MDBreadCrumb::updatePath(const QString& path) {

    QDir pathDir(path);

    QString nativePath = QDir::fromNativeSeparators(pathDir.absolutePath());
    QString nativeRoot = QDir::fromNativeSeparators(rootPath.absolutePath());

    if (currentPath.compare(nativePath) == 0)
        return;

    dPtr->removeAll();
    QList<Item> items;

    // verifico se rootPath e' padre di nativePath
    while (nativePath.toLower().indexOf(nativeRoot.toLower()) != -1) {

        Item currentItem;
        if (pathDir == rootPath)
            currentItem.key = rootName;
        else
            currentItem.key = pathDir.dirName();
        currentItem.value = pathDir.absolutePath();

        items.push_front(currentItem);
        pathDir.cdUp();
        nativePath = QDir::fromNativeSeparators(pathDir.absolutePath());

    }

    currentPath = nativePath;

    for (Item& i: items)
        dPtr->addItem(i.key, i.value);

    this->updateButtonsUi();

}

void MDBreadCrumb::setRoot(const QString& rootName, const QString& rootPath) {
    this->setRoot(rootName, QDir(rootPath));
}

void MDBreadCrumb::setRoot(const QString& rootName, const QDir& rootPath) {
    this->rootName = rootName;
    this->rootPath = rootPath;
}

QString MDBreadCrumb::getRootName() const {
    return rootName;
}

void MDBreadCrumb::addItem(const QString& item, const QString& path) {

    dPtr->addItem(item, path);
    this->updateButtonsUi();
    this->setupButtonsSignalsAndSlots();
}

void MDBreadCrumb::removeLast() {

    dPtr->removeLast();
    this->updateButtonsUi();
    this->setupButtonsSignalsAndSlots();

}

void MDBreadCrumb::setupSignalsAndSlots() {

    // altri segnali da gestire non riguardanti i pulsanti dinamici ivanno inseriti qui
    setupButtonsSignalsAndSlots();


}

void MDBreadCrumb::setupButtonsSignalsAndSlots() {

    // prima di tutto, rimuovo tutte le connessioni (i pulsanti vengono ricreati ogni volta)
    for (QMetaObject::Connection& c: connections)
        disconnect(c);
    connections.clear();

    int signalIndex = 0;
    for (int i=0; i<this->count(); ++i) {

        QWidget* widget = this->itemWidget(this->item(i));
        QPushButton* pushButton = qobject_cast<QPushButton*>(widget);

        if (QMenu* menu = pushButton->menu()) {

            int countAction = 0;
            for (QAction* action : menu->actions()) {
                QMetaObject::Connection c = connect(action, &QAction::triggered, [&, signalIndex]() {
                    this->manageClickEvent(signalIndex);
                });
                connections.append(c);
                ++signalIndex;
            }

        } else {

            if (widget) {
                QMetaObject::Connection c = connect(pushButton, &QPushButton::clicked, [&, signalIndex]() {
                    this->manageClickEvent(signalIndex);
                });
                connections.append(c);
            }

            ++signalIndex;
        }

    }

}

void MDBreadCrumb::setupUi() {

    this->setEditTriggers(EditTrigger::NoEditTriggers);
    this->setSelectionMode(SelectionMode::NoSelection);
    this->setFlow(Flow::LeftToRight);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setUniformItemSizes(false);

}

void MDBreadCrumb::updateButtonsUi() {

    this->clear();

    if (dPtr->isEmpty())
        return;

    QString root = dPtr->getRootKey();

    QPushButton* rootButton = new QPushButton(root, this);
    rootButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QListWidgetItem* rootWidgetItem = new QListWidgetItem(this);
    rootWidgetItem->setSizeHint(rootButton->minimumSizeHint());
    static_cast<QListWidget*>(this)->addItem(rootWidgetItem);
    this->setItemWidget(rootWidgetItem, rootButton);

    if (dPtr->hasCollapsedGroup()) {

        QPushButton* collapsedGroupButton = new QPushButton(DOTS, this);
        collapsedGroupButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        QListWidgetItem* collapsedGroupWidgetItem = new QListWidgetItem(this);
        collapsedGroupWidgetItem->setSizeHint(collapsedGroupButton->minimumSizeHint());
        static_cast<QListWidget*>(this)->addItem(collapsedGroupWidgetItem);
        this->setItemWidget(collapsedGroupWidgetItem, collapsedGroupButton);

        QStringList collapsedGroup = dPtr->getCollapsedGroup();
        QMenu* menu = new QMenu(collapsedGroupButton);
        for (const QString& itemCollapse: collapsedGroup)
            menu->addAction(itemCollapse);
        collapsedGroupButton->setMenu(menu);

    }

    QStringList tail = dPtr->getTail();

    if (!tail.isEmpty()) {

        for (const QString itemTail: tail) {
            QPushButton* itemTailButton = new QPushButton(itemTail, this);
            itemTailButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            QListWidgetItem* itemTailWidgetItem = new QListWidgetItem(this);
            itemTailWidgetItem->setSizeHint(itemTailButton->minimumSizeHint());
            static_cast<QListWidget*>(this)->addItem(itemTailWidgetItem);
            this->setItemWidget(itemTailWidgetItem, itemTailButton);
        }

    }

    this->setupButtonsSignalsAndSlots();

}

void MDBreadCrumb::manageClickEvent(int index) {

    QString item = dPtr->getKey(index);
    QString path = dPtr->getValue(index);

    emit itemClicked(item);
    emit pathClicked(path);

}
