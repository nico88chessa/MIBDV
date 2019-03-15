#ifndef ITEMDETAILBEANS_HPP
#define ITEMDETAILBEANS_HPP

#include <QMetaType>
#include <QString>
#include <Point.hpp>
#include <configure.h>

enum class ItemDetailType : int {
    NONE,
    FILTER,
    FOLDER
};



/*
 *  I T E M  D E T A I L  N O N E
 */

class ItemDetailNone {
public:
    using Ptr = ItemDetailNone*;
    using ConstPtr = const ItemDetailNone*;

public:
    ItemDetailNone() { }
    ~ItemDetailNone() { }

};

Q_DECLARE_METATYPE(ItemDetailNone)


/*
 *  I T E M  D E T A I L  F I L T R O  B E A N
 */

class ItemDetailFilterBean {
public:
    using Ptr = ItemDetailFilterBean*;
    using ConstPtr = const ItemDetailFilterBean*;

private:
    QString name;
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

};

Q_DECLARE_METATYPE(ItemDetailFilterBean)



/*
 *  I T E M  D E T A I L  F O  L D E R  B E A N
 */

class ItemDetailFolderBean {
public:
    using Ptr = ItemDetailFolderBean*;
    using ConstPtr = const ItemDetailFolderBean*;

private:
    QString name;
    int numItems;

public:
    ItemDetailFolderBean() { }
    ~ItemDetailFolderBean() { }

    QString getName() const { return name; }
    void setName(const QString& value) { name = value; }

    int getNumItems() const { return numItems; }
    void setNumItems(int value) { numItems = value; }

};


Q_DECLARE_METATYPE(ItemDetailFolderBean)

#endif // ITEMDETAILBEANS_HPP
