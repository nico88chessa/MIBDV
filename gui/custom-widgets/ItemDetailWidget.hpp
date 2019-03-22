#ifndef ITEMDETAILWIDGET_HPP
#define ITEMDETAILWIDGET_HPP

#include <QGridLayout>
#include <QStackedWidget>
#include <QMap>
#include <QDataWidgetMapper>

#include "MDLineEdit.hpp"


class ItemDetailModel;
class ItemDetailDelegate;

/*
 * I T E M D E T A I L T Y P E
 */

enum class ItemDetailType : int {
    NONE,
    FILTER,
    FOLDER
};


/*
 *  I T E M  D E T A I L  W I D G E T
 */

class ItemDetailWidget : public QWidget {
    Q_OBJECT

    friend class ItemDetailDelegate;

public:
    using Ptr = ItemDetailWidget*;
    using ConstPtr = const ItemDetailWidget*;

private:
    QStackedWidget* stackedWidget;
    QGridLayout* gridLayout;

    QMap<ItemDetailType, QWidget*> map; // mappa itemDetailEnum / widget

    ItemDetailModel* model;
    ItemDetailDelegate* delegate;
    QDataWidgetMapper* mapper;

public:
    ItemDetailWidget(QWidget* parent = Q_NULLPTR);
    void updateWidgetValue(const QVariant& value);

public slots:
    void updateModelData(const QStringList& itemsPath);
    void setCurrentPath(const QString& itemPath);

protected:
    void showWidget(ItemDetailType type);
    void showWidget(const QVariant& value);
    void setupSignalsAndSlots();
    void setupUi();

};

#endif // ITEMDETAILWIDGET_HPP
