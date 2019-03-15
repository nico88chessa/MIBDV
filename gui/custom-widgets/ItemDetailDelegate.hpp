#ifndef ITEMDETAILDELEGATE_HPP
#define ITEMDETAILDELEGATE_HPP

#include <QStyledItemDelegate>


class ItemDetailDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using Ptr = ItemDetailDelegate*;
    using ConstPtr = const ItemDetailDelegate*;

public:
    explicit ItemDetailDelegate(QObject *parent = Q_NULLPTR);

    void setEditorData(QWidget* editor, const QModelIndex& index) const;

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

};

#endif // ITEMDETAILDELEGATE_HPP
