#ifndef ITEMDETAILWIDGET_HPP
#define ITEMDETAILWIDGET_HPP

#include <QGridLayout>
#include <QStackedWidget>
#include <QMap>

#include "ItemDetailBeans.hpp"
#include "MDLineEdit.hpp"


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
    QMap<ItemDetailType, QWidget*> map; // mappa itemDetailEnum / widget

    QGridLayout* gridLayout;
    MDLineEdit* testLineEdit;
    MDLineEdit* testLineEdit2;
    QSpacerItem* verticalSpacer;

public:
    ItemDetailWidget(QWidget* parent = Q_NULLPTR);

    void showWidget(ItemDetailType type);
    void showWidget(const QVariant& value);
    void updateWidgetValue(const QVariant& value);

protected:
    void setupSignalsAndSlots();
    void setupUi();

};



/*
 *  I T E M  D E T A I L  F I L T E R  W I D G E T
 */

class ItemDetailFilterWidget : public QWidget {
    Q_OBJECT

    friend class ItemDetailWidget;

public:
    using Ptr = ItemDetailFilterWidget*;
    using ConstPtr = const ItemDetailFilterWidget*;

private:
    QGridLayout* gridLayout;
    MDLineEdit* leNumberOfPoints;
    MDLineEdit* minPoint;
    MDLineEdit* maxPoint;
    QSpacerItem* verticalSpacer;

public:
    ItemDetailFilterWidget(QWidget* parent = Q_NULLPTR);
    void model2View(const ItemDetailFilterBean& bean);

protected:
    void setupSignalsAndSlots();
    void setupUi();
};



/*
 *  I T E M  D E T A I L  F O L D E R  W I D G E T
 */

class ItemDetailFolderWidget : public QWidget {
    Q_OBJECT

    friend class ItemDetailWidget;

public:
    using Ptr = ItemDetailFolderWidget*;
    using ConstPtr = const ItemDetailFolderWidget*;

private:
    QGridLayout* gridLayout;
    MDLineEdit* leName;
    MDLineEdit* leItemsCount;
    QSpacerItem* verticalSpacer;

public:
    ItemDetailFolderWidget(QWidget* parent = Q_NULLPTR);
    void model2View(const ItemDetailFolderBean& bean);

protected:
    void setupSignalsAndSlots();
    void setupUi();

};

#endif // ITEMDETAILWIDGET_HPP
