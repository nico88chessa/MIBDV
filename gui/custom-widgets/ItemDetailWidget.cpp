#include "ItemDetailWidgetPrivate.hpp"
#include "ItemDetailWidget.hpp"

#include <json/FilterJsonParser.hpp>
#include <QDir>


/*
 *  I T E M  D E T A I L  F I L T E R  W I D G E T
 */

ItemDetailFilterWidget::ItemDetailFilterWidget(QWidget* parent) {

    this->setupUi();

}

void ItemDetailFilterWidget::model2View(const ItemDetailFilterBean& bean) {

    using namespace PROGRAM_NAMESPACE;

    leNumberOfPoints->setText(QString::number(bean.getNumberOfPoints()));
    leName->setText(bean.getName());
    minPoint->setText(QString::number(bean.getMinPoint().getX()) + " - " + QString::number(bean.getMinPoint().getY()));
    maxPoint->setText(QString::number(bean.getMaxPoint().getX()) + " - " + QString::number(bean.getMaxPoint().getY()));

}

void ItemDetailFilterWidget::setupSignalsAndSlots() {

}

void ItemDetailFilterWidget::setupUi() {

    gridLayout = new QGridLayout(this);

    leName = new MDLineEdit(this);
    leName->setLabel("File name");
    leName->setReadOnly(true);

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

    gridLayout->addWidget(leName, 0, 0, 1, 1);
    gridLayout->addWidget(leNumberOfPoints, 1, 0, 1, 1);
    gridLayout->addWidget(minPoint, 2, 0, 1, 1);
    gridLayout->addWidget(maxPoint, 3, 0, 1, 1);
    gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

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
    leItemsCount->setLabel("Num. Files");
    leItemsCount->setReadOnly(true);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addWidget(leName, 0, 0, 1, 1);
    gridLayout->addWidget(leItemsCount, 1, 0, 1, 1);
    gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);

}


/*
 * I T E M D E T A I L M O D E L
 */

ItemDetailModel::ItemDetailModel(QObject* parent) :
    QAbstractListModel(parent) {

    this->clear();

}

int ItemDetailModel::rowCount(const QModelIndex& parent) const {
    return items.count();
}

QVariant ItemDetailModel::data(const QModelIndex& index, int role) const {

    if (!(index.row() < this->rowCount()))
        return QVariant();

    const QVariant& bean = items.at(index.row());
    return QVariant::fromValue(bean);

}

void ItemDetailModel::addData(const QVariant& item) {

    if (!checkData(item))
        return;

    int size = this->rowCount();
    this->beginInsertRows(QModelIndex(), size, size);
    items.append(item);
    this->endInsertRows();

}

void ItemDetailModel::addFolderDetail(const ItemDetailFolderBean& item) {
    const QVariant i = QVariant::fromValue(item);
    this->addData(i);
}

void ItemDetailModel::addFilterDetail(const ItemDetailFilterBean& item) {
    const QVariant i = QVariant::fromValue(item);
    this->addData(i);
}

void ItemDetailModel::clear() {

    this->beginResetModel();
    this->items.clear();
    this->endResetModel();

    // aggiungo una riga vuota
    this->beginInsertRows(QModelIndex(), 0, 0);
    items.append(QVariant());
    this->endInsertRows();

}

QModelIndex ItemDetailModel::findPath(const QString& path) const {

    int row = 0;
    bool found = false;
    QModelIndex index;

    for (int i = 0; i<this->rowCount() && !found; ++i) {

        index = this->index(i, 0);
        QVariant data = index.data();

        if (data.canConvert<ItemDetailFilterBean>()) {

            ItemDetailFilterBean bean = data.value<ItemDetailFilterBean>();
            if (bean.getPath().compare(path) ==0 )
                found = true;

        } else if (data.canConvert<ItemDetailFolderBean>()) {

            ItemDetailFolderBean bean = data.value<ItemDetailFolderBean>();
            if (bean.getPath().compare(path) ==0 )
                found = true;

        }

    }

    if (found)
        return index;
    return this->index(0, 0);

}

bool ItemDetailModel::checkData(const QVariant& data) {

    bool isDataOk = data.canConvert<ItemDetailFilterBean>() ||
            data.canConvert<ItemDetailFolderBean>();

    return isDataOk;

}


/*
 * I T E M D E T A I L D E L E G A T E
 */

ItemDetailDelegate::ItemDetailDelegate(QObject* parent) :
    QStyledItemDelegate(parent) {
}

void ItemDetailDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {

    QVariant data = index.data();
    ItemDetailWidget::Ptr itemDetailWidget = qobject_cast<ItemDetailWidget::Ptr>(editor);

    if (itemDetailWidget) {
        itemDetailWidget->updateWidgetValue(data);
        itemDetailWidget->showWidget(data);
        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);

}


/*
 *  I T E M  D E T A I L  W I D G E T
 */

ItemDetailWidget::ItemDetailWidget(QWidget* parent) :
    QWidget(parent), map() {

    this->setupUi();
    this->setupSignalsAndSlots();

    mapper = new QDataWidgetMapper(this);
    delegate = new ItemDetailDelegate(this);
    model = new ItemDetailModel();

    mapper->setItemDelegate(delegate);
    mapper->setModel(model);
    mapper->addMapping(this, 0);

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

void ItemDetailWidget::updateModelData(const QStringList& itemsPath) {

    this->model->clear();

    using namespace PROGRAM_NAMESPACE;
    QScopedPointer<IAbstractJsonParser> parser(new FilterJsonParser());

    for (const QString& item: itemsPath) {

        QFileInfo fileInfo(item);
        if (fileInfo.exists()) {

            if (fileInfo.isDir()) {

                QDir dir(item);
                dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::AllDirs);

                ItemDetailFolderBean folder;
                folder.setName(fileInfo.fileName());
                folder.setNumItems(dir.count()/*fileInfo.size()*/);
                folder.setPath(fileInfo.absoluteFilePath());
                this->model->addFolderDetail(folder);

            } else if (fileInfo.isFile()) {

                QString filePath = fileInfo.absoluteFilePath();
                QFile file(filePath);
                if (!file.open(QFile::ReadOnly | QFile::Text)) {
                    traceErr() << "Impossibile aprire il file:" << fileInfo.fileName();
                    continue;
                }

                QString bytes = file.readAll();
                file.close();

                Filter flt;
                JsonParserError err;
                if (!(err = parser->decodeJsonHeaderOnly(bytes.toUtf8(), &flt) == JSON_PARSER_NO_ERROR)) {
                    traceErr() << "Errore nel parsing del file:" << fileInfo.fileName();
                    traceErr() << "Errore parser:" << err;
                    continue;
                }

                ItemDetailFilterBean filter;

                filter.setName(fileInfo.fileName());
                filter.setPath(fileInfo.absoluteFilePath());
                filter.setMinPoint(flt.getMin());
                filter.setMaxPoint(flt.getMax());
                filter.setNumberOfPoints(flt.getNumOfPoints());

                this->model->addFilterDetail(filter);

            }

        }

    }

}

void ItemDetailWidget::setCurrentPath(const QString& itemPath) {

    QModelIndex index = this->model->findPath(itemPath);
    this->mapper->setCurrentModelIndex(index);

}

void ItemDetailWidget::setupSignalsAndSlots() {
    // do nothing
}

void ItemDetailWidget::setupUi() {

    gridLayout = new QGridLayout(this);

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
