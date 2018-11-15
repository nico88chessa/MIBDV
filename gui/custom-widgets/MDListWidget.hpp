#ifndef MDLISTWIDGET_HPP
#define MDLISTWIDGET_HPP

#include <QLabel>
#include <QListWidget>
#include <QStyledItemDelegate>
/*
class MDListWidgetItem : public QListWidgetItem {
public:
    using Ptr = MDListWidgetItem*;
    using ConstPtr = const MDListWidgetItem*;

public:
    explicit MDListWidgetItem(QListWidget *view = Q_NULLPTR, int type = Type);


};

class MDListItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using Ptr = MDListItemDelegate*;
    using ConstPtr = const MDListItemDelegate*;

public:
    explicit MDListItemDelegate(QObject *parent = Q_NULLPTR);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};*/

class MDCustomItem : public QLabel {
    Q_OBJECT
    Q_PROPERTY(QString iconPath READ getIconPath WRITE setIconPath)
    Q_PROPERTY(int iconWidth READ getIconWidth WRITE setIconWidth)
    Q_PROPERTY(int iconHeight READ getIconHeight WRITE setIconHeight)
    Q_PROPERTY(int iconPaddingRight READ getIconPaddingRight WRITE setIconPaddingRight)

public:
    using Ptr = MDCustomItem*;
    using ConstPtr = const MDCustomItem*;

public:
    explicit MDCustomItem(QWidget* parent = nullptr);
    explicit MDCustomItem(const QString& text, const QString& iconPath);

    QString getIconPath() const;
    void setIconPath(const QString& value);
    QSize sizeHint() const;

    int getIconWidth() const;
    void setIconWidth(int value);

    int getIconHeight() const;
    void setIconHeight(int value);

    int getIconPaddingRight() const;
    void setIconPaddingRight(int value);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QString iconPath;
    int iconWidth;
    int iconHeight;
    int iconPaddingRight;

};

class MDListWidget : public QListWidget {
    Q_OBJECT

public:
    using Ptr = MDListWidget*;
    using ConstPtr = const MDListWidget*;

public:

    explicit MDListWidget(QObject* parent = Q_NULLPTR);


};

#endif // MDLISTWIDGET_HPP
