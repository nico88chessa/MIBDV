#ifndef ALERTFRAMELOGIC_HPP
#define ALERTFRAMELOGIC_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>

#include <data/Error.hpp>
#include <common/Utils.hpp>

#include <ErrorHandler.hpp>


namespace PN = PROGRAM_NAMESPACE;


/*
 * A L E R T   I T E M   M O D E L
 */

class AlertItemModel : public QAbstractListModel {
    Q_OBJECT

public:
    using Ptr = AlertItemModel*;
    using ConstPtr = const AlertItemModel*;

private:
    QList<PN::Error> errors;

public:
    explicit AlertItemModel(QObject* parent = Q_NULLPTR);
    ~AlertItemModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;

    void clear();
    bool addError(const PN::Error& item);
    void updateErrors(const QList<PN::Error> refreshList);
    QList<PN::Error> getErrors() const;

signals:
//    void modelUpdated();


    // QAbstractItemModel interface
public:
    void sort(int column, Qt::SortOrder order);
};



/*
 *  A L E R T   I T E M   D E L E G A T E
 */

class AlertItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using Ptr = AlertItemDelegate*;
    using ConstPtr = const AlertItemDelegate*;

public:
    explicit AlertItemDelegate(QObject* parent = Q_NULLPTR);
    ~AlertItemDelegate();

public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};



/*
 *  A L E R T   F R A M E   L O G I C
 */

class AlertFrame;

class AlertFrameLogic : public QObject {
    Q_OBJECT
    friend class AlertFrame;

public:
    using Ptr = AlertFrameLogic*;
    using ConstPtr = const AlertFrameLogic*;

private:
    AlertFrame* qPtr;
    AlertItemModel* model;
    QWeakPointer<PROGRAM_NAMESPACE::ErrorManager> errorManager;

private slots:
    void updateModel(const QList<PROGRAM_NAMESPACE::Error>& updateErrors);

public:
    AlertFrameLogic();
    ~AlertFrameLogic();

};

#endif // ALERTFRAMELOGIC_HPP
