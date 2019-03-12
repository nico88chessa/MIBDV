#include "MDBreadCrumb.hpp"

#include <QMenu>
#include <QPushButton>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QLabel>

using namespace internal;

const char* MDBreadCrumbPrivate::PATH_DELIMITER = "//";
const char* MDBreadCrumb::DOTS = "...";

/*** MDBreadCrumbPrivate ***/

MDBreadCrumbPrivate::MDBreadCrumbPrivate(const QString& rootName, int tailSize) :
    maxTailSize(tailSize), items() {

    addItem(rootName);

}

void MDBreadCrumbPrivate::addItem(const QString& item) {

    Item newItem;
    newItem.itemKey = item;
    if (size() == 0)
        newItem.itemValue.append(item);
    else {
        newItem.itemValue = items.last().itemValue;
        newItem.itemValue.append(item);
    }
    items.append(newItem);

}

QStringList MDBreadCrumbPrivate::getItem(int index) const {

    if (index < size())
        return items.at(index).itemValue;
    return QStringList();

}

QString MDBreadCrumbPrivate::getKey(int index) const {

    if (index < size())
        return items.at(index).itemKey;
    return QString();

}

QString MDBreadCrumbPrivate::getPath(int index) const {

    QStringList listItem = this->getItem(index);
    QString res;
    int i = 0;
    for (QString& str: listItem) {
        res.append(str);
        if (!(i == listItem.size() - 1))
            res.append(PATH_DELIMITER);
        ++i;
    }
    return res;
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

bool MDBreadCrumbPrivate::hasCollapsedGroup() const {
    return (items.size() - 1) > maxTailSize;
}

QString MDBreadCrumbPrivate::getRoot() const {
    if (!items.isEmpty())
        return items.at(0).itemKey;
    return QString();
}

QStringList MDBreadCrumbPrivate::getCollapsedGroup() const {

    QStringList temp;
    if (hasCollapsedGroup()) {
        int collapsedGroupSize = size() - 1 - (maxTailSize - 1);
//        temp = items.mid(1, collapsedGroupSize);
        temp = items.last().itemValue.mid(1, collapsedGroupSize);
//        temp = items.at(size() - maxTailSize).itemValue;
    }
    return temp;

}

QStringList MDBreadCrumbPrivate::getTail() const {

    QStringList temp;

    if (hasCollapsedGroup()) {
        int index = this->size() - maxTailSize + 1;
        int count = maxTailSize - 1;
//        temp = items.mid(index, count);
        temp = items.last().itemValue.mid(index, count);
    } else {
        int count = this->size() - 1;
//        temp = items.mid(1, count);
        temp = items.last().itemValue.mid(1, count);
    }

    return temp;
}

int MDBreadCrumbPrivate::getMaxTailSize() const {
    return maxTailSize;
}




/*** MDBreadCrumb ***/

MDBreadCrumb::MDBreadCrumb(QWidget* parent) :
    dPtr(new internal::MDBreadCrumbPrivate()), QListWidget(parent) {

    setupUi();
    updateButtonsUi();
    setupSignalsAndSlots();

}

void MDBreadCrumb::addItem(const QString& item) {

    dPtr->addItem(item);
    this->updateButtonsUi();
    this->setupSignalsAndSlots();
}

void MDBreadCrumb::removeLast() {

    dPtr->removeLast();
    this->updateButtonsUi();
    this->setupSignalsAndSlots();

}

void MDBreadCrumb::setupSignalsAndSlots() {

//    connect(this, &MDBreadCrumb::itemClicked, this, &MDBreadCrumb::manageClickEvent);
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
    QString root = dPtr->getRoot();

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

}

void MDBreadCrumb::manageClickEvent(int index) {

    QString path = dPtr->getPath(index);
    QString item = dPtr->getKey(index);

    emit itemClicked(item);
    emit pathClicked(path);

}

//void MDBreadCrumb::manageClickEvent(QListWidgetItem* item) {

//    QObject* sender = QObject::sender();


//    QWidget* w = this->itemWidget(item);
//    QPushButton* pb = qobject_cast<QPushButton*>(w);
//    if (pb) {
//        pb->showMenu();
//    }
//}

//void MDBreadCrumb::mousePressEvent(QMouseEvent* event) {

//    if (event->button() == Qt::LeftButton){
//        QListWidgetItem* widgetItem = this->itemAt(event->pos());
//        QWidget* w = this->itemWidget(widgetItem);
//        QPushButton* pb = qobject_cast<QPushButton*>(w);
//        if (pb) {
//            pb->showMenu();
//        }

//    } else {
//        QListWidget::mousePressEvent(event);
//    }
//}
