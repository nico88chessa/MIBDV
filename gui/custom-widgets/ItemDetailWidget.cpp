#include "ItemDetailWidget.hpp"
#include <Point.hpp>

/*
 *  I T E M  D E T A I L  W I D G E T
 */

ItemDetailWidget::ItemDetailWidget(QWidget* parent) :
    QWidget(parent), map() {

    this->setupUi();
    this->setupSignalsAndSlots();

}

void ItemDetailWidget::showWidget(ItemDetailType type) {

    QWidget* w = map[type];
    if (w)
        stackedWidget->setCurrentWidget(w);

}

void ItemDetailWidget::showWidget(const QVariant& value) {

    if (value.canConvert<ItemDetailFilterBean>())
        showWidget(ItemDetailType::FILTER);
    else if (value.canConvert<ItemDetailFolderBean>())
        showWidget(ItemDetailType::FOLDER);
    else
        showWidget(ItemDetailType::NONE);

}

void ItemDetailWidget::updateWidgetValue(const QVariant& value) {

    if (value.canConvert<ItemDetailFilterBean>()) {

        QWidget* w = map[ItemDetailType::FILTER];
        if (w)
            static_cast<ItemDetailFilterWidget*>(w)->model2View(value.value<ItemDetailFilterBean>());

    } else if (value.canConvert<ItemDetailFolderBean>()) {

        QWidget* w = map[ItemDetailType::FOLDER];
        if (w)
            static_cast<ItemDetailFolderWidget*>(w)->model2View(value.value<ItemDetailFolderBean>());

    }

}

void ItemDetailWidget::setupSignalsAndSlots() {
    // do nothing
}

void ItemDetailWidget::setupUi() {

    gridLayout = new QGridLayout(this);
//    this->setLayout(gridLayout);

    stackedWidget = new QStackedWidget(this);

    QWidget* widget = new QWidget(this);
    stackedWidget->addWidget(widget);
    map[ItemDetailType::NONE] = widget;

    ItemDetailFilterWidget::Ptr filterWidget = new ItemDetailFilterWidget(this);
    stackedWidget->addWidget(filterWidget);
    map[ItemDetailType::FILTER] = filterWidget;

    ItemDetailFolderWidget::Ptr folderWidget = new ItemDetailFolderWidget(this);
    stackedWidget->addWidget(folderWidget);
    map[ItemDetailType::FOLDER] = folderWidget;

    gridLayout->addWidget(stackedWidget);

}



/*
 *  I T E M  D E T A I L  F I L T E R  W I D G E T
 */

ItemDetailFilterWidget::ItemDetailFilterWidget(QWidget* parent) {

    this->setupUi();

}

void ItemDetailFilterWidget::model2View(const ItemDetailFilterBean& bean) {

    using namespace PROGRAM_NAMESPACE;

    leNumberOfPoints->setText(QString::number(bean.getNumberOfPoints()));
//    leNumberOfPoints->setText(bean.getName());
    minPoint->setText(bean.getMinPoint().getX() + " - " + bean.getMinPoint().getY());
    maxPoint->setText(bean.getMaxPoint().getX() + " - " + bean.getMaxPoint().getY());

}

void ItemDetailFilterWidget::setupSignalsAndSlots() {

}

void ItemDetailFilterWidget::setupUi() {

    gridLayout = new QGridLayout(this);

    leNumberOfPoints = new MDLineEdit(this);
    leNumberOfPoints->setLabel("Number of Points");
    leNumberOfPoints->setReadOnly(true);
    minPoint = new MDLineEdit(this);
    minPoint->setLabel("Min Point");
    minPoint->setReadOnly(true);
    maxPoint = new MDLineEdit(this);
    maxPoint->setLabel("Max Point");
    maxPoint->setReadOnly(true);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addWidget(leNumberOfPoints, 0, 0, 1, 1);
    gridLayout->addWidget(minPoint, 1, 0, 1, 1);
    gridLayout->addWidget(maxPoint, 2, 0, 1, 1);
    gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);

//    this->setLayout(gridLayout);

}



/*
 *  I T E M  D E T A I L  F O L D E R  W I D G E T
 */

ItemDetailFolderWidget::ItemDetailFolderWidget(QWidget* parent) {

    this->setupUi();

}

void ItemDetailFolderWidget::model2View(const ItemDetailFolderBean& bean) {

    leName->setText(bean.getName());
    leItemsCount->setText(QString::number(bean.getNumItems()));

}

void ItemDetailFolderWidget::setupSignalsAndSlots() {

}

void ItemDetailFolderWidget::setupUi() {

    gridLayout = new QGridLayout(this);

    leName = new MDLineEdit(this);
    leName->setLabel("Name");
    leName->setReadOnly(true);
    leItemsCount = new MDLineEdit(this);
    leItemsCount->setLabel("Size");
    leItemsCount->setReadOnly(true);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addWidget(leName, 0, 0, 1, 1);
    gridLayout->addWidget(leItemsCount, 1, 0, 1, 1);
    gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);

//    this->setLayout(gridLayout);

}
