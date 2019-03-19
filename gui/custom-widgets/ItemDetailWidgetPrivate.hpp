#ifndef ITEMDETAILWIDGETPRIVATE_HPP
#define ITEMDETAILWIDGETPRIVATE_HPP

#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QMetaType>
#include <QString>

#include <Point.hpp>
#include <QGridLayout>
#include "MDLineEdit.hpp"
#include <configure.h>


/*
 *  I T E M  D E T A I L  N O N E
 */

//class ItemDetailNone {
//public:
//    using Ptr = ItemDetailNone*;
//    using ConstPtr = const ItemDetailNone*;

//public:
//    ItemDetailNone() { }
//    ~ItemDetailNone() { }

//};

//Q_DECLARE_METATYPE(ItemDetailNone)


/*
 *  I T E M D E T A I L F I L T R O B E A N
 */

class ItemDetailFilterBean {
public:
    using Ptr = ItemDetailFilterBean*;
    using ConstPtr = const ItemDetailFilterBean*;

private:
    QString name;
    QString path;
    quint32 numberOfPoints;
    PROGRAM_NAMESPACE::PointI minPoint;
    PROGRAM_NAMESPACE::PointI maxPoint;

public:
    ItemDetailFilterBean() { }
    ~ItemDetailFilterBean() { }

    QString getName() const { return name; }
    void setName(const QString& value) { name = value; }

    quint32 getNumberOfPoints() const { return numberOfPoints; }
    void setNumberOfPoints(const quint32& value) { numberOfPoints = value; }

    PROGRAM_NAMESPACE::PointI getMinPoint() const { return minPoint; }
    void setMinPoint(const PROGRAM_NAMESPACE::PointI& value) { minPoint = value; }

    PROGRAM_NAMESPACE::PointI getMaxPoint() const { return maxPoint; }
    void setMaxPoint(const PROGRAM_NAMESPACE::PointI& value) { maxPoint = value; }

    QString getPath() const { return path; }
    void setPath(const QString& value) { path = value; }

};

Q_DECLARE_METATYPE(ItemDetailFilterBean)


/*
 *  I T E M D E T A I L F O L D E R B E A N
 */

class ItemDetailFolderBean {
public:
    using Ptr = ItemDetailFolderBean*;
    using ConstPtr = const ItemDetailFolderBean*;

private:
    QString name;
    QString path;
    int numItems;

public:
    ItemDetailFolderBean() { }
    ~ItemDetailFolderBean() { }

    QString getName() const { return name; }
    void setName(const QString& value) { name = value; }

    int getNumItems() const { return numItems; }
    void setNumItems(int value) { numItems = value; }

    QString getPath() const { return path; }
    void setPath(const QString& value) { path = value; }

};

Q_DECLARE_METATYPE(ItemDetailFolderBean)


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
    MDLineEdit* leName;
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


/*
 *  I T E M D E T A I L M O D E L
 */

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
    void clear();
    QModelIndex findPath(const QString& path) const;

private:
    bool checkData(const QVariant& data);

};


/*
 *  I T E M D E T A I L D E L E G A T E
 */

class ItemDetailDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using Ptr = ItemDetailDelegate*;
    using ConstPtr = const ItemDetailDelegate*;

public:
    explicit ItemDetailDelegate(QObject *parent = Q_NULLPTR);

    void setEditorData(QWidget* editor, const QModelIndex& index) const;

};


#endif // ITEMDETAILWIDGETPRIVATE_HPP
