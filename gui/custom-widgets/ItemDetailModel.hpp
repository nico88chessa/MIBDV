#ifndef ITEMDETAILMODEL_HPP
#define ITEMDETAILMODEL_HPP

#include <QAbstractListModel>

class ItemDetailFolderBean;
class ItemDetailFilterBean;

class ItemDetailModel : public QAbstractListModel {
    Q_OBJECT
public:
    using Ptr = ItemDetailModel*;
    using ConstPtr = const ItemDetailModel*;

private:
    QList<QVariant> items;

public:
    explicit ItemDetailModel(QObject *parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role) const;
    void addData(const QVariant& item);
    void addFolderDetail(const ItemDetailFolderBean& item);
    void addFilterDetail(const ItemDetailFilterBean& item);

private:
    bool checkData(const QVariant& data);

};

#endif // ITEMDETAILMODEL_HPP
