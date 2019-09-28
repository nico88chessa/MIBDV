#ifndef LEFTFRAMELOGIC_HPP
#define LEFTFRAMELOGIC_HPP

#include <QAbstractListModel>
#include <QObject>
#include <QStyledItemDelegate>


class LeftFrame;

/*
 * M E N U   I T E M
 */

class MenuItem {
public:
    using Ptr = MenuItem*;
    using ConstPtr = const MenuItem*;

private:
    QString key;
    QString label;
    bool isEnabled; // NOTE NIC 13/08/2019 - per ora non usato

public:

    MenuItem() : MenuItem("", "") { }
    MenuItem(const QString& key, const QString& label, bool isEnabled = true) {
        this->key = key;
        this->label = label;
        this->isEnabled = isEnabled;
    }

    QString getKey() const { return key; }
    void setKey(const QString& value) { key = value; }

    QString getLabel() const { return label; }
    void setLabel(const QString& value) { label = value; }

    bool getIsEnabled() const { return isEnabled; }
    void setIsEnabled(bool value) { isEnabled = value; }

};

Q_DECLARE_METATYPE(MenuItem)



/*
 * M E N U   I T E M   D E L E G A T E
 */

class MenuItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using Ptr = MenuItemDelegate*;
    using ConstPtr = const MenuItemDelegate*;

public:
    explicit MenuItemDelegate(QObject* parent = Q_NULLPTR);
    ~MenuItemDelegate();

public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

};



/*
 * M E N U   I T E M   M O D E L
 */

class MenuItemModel : public QAbstractListModel {
    Q_OBJECT

public:
    using Ptr = MenuItemModel*;
    using ConstPtr = const MenuItemModel*;

private:
    QList<MenuItem> items;

public:
    explicit MenuItemModel(QObject* parent = Q_NULLPTR);
    virtual ~MenuItemModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    bool addItem(const MenuItem& item);

};



/*
 * L E F T   F R A M E   L O G I C
 */

class LeftFrameLogic : public QObject {
    Q_OBJECT

public:
    using Ptr = LeftFrameLogic*;
    using ConstPtr = const LeftFrameLogic*;

    friend class LeftFrame;
    MenuItemModel* model;
    MenuItemDelegate* delegate;

private:
    LeftFrame* qPtr;

public:
    LeftFrameLogic();
    virtual ~LeftFrameLogic() override;

};

#endif // LEFTFRAMELOGIC_HPP
