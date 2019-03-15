#include "ItemDetailModel.hpp"
#include "ItemDetailBeans.hpp"


ItemDetailModel::ItemDetailModel(QObject* parent) :
    QAbstractListModel(parent) {
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

bool ItemDetailModel::checkData(const QVariant& data) {

    bool isDataOk = data.canConvert<ItemDetailFilterBean>() ||
            data.canConvert<ItemDetailFolderBean>();

    return isDataOk;

}
