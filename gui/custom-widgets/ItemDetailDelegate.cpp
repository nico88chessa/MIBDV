#include "ItemDetailDelegate.hpp"

#include "ItemDetailWidget.hpp"

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

//    if (data.canConvert<ItemDetailBean>()) {

//        ItemDetailBean bean = qvariant_cast<ItemDetailBean>(data);
//        ItemDetailWidget* itemWidget = qobject_cast<ItemDetailWidget*>(editor);
//        itemWidget->testLineEdit->setText(bean.getText1());
//        itemWidget->testLineEdit2->setText(bean.getText2());
//        return;

//    }

    QStyledItemDelegate::setEditorData(editor, index);

}

void ItemDetailDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {

    int ciao = 0;
    ciao++;

}
