#ifndef MDBREADCRUMB_HPP
#define MDBREADCRUMB_HPP

#include <QListWidget>
#include <QStringList>
#include <QVector>
#include <QDir>

namespace internal {

struct Item {
public:
    QString key; // singola stringa
    QString value; // prefisso contenente tutte le stringhe 'genitori'
};

class MDBreadCrumbPrivate {
public:
    using Ptr = MDBreadCrumbPrivate*;
    using ConstPtr = const MDBreadCrumbPrivate*;

public:
    static constexpr const char* ROOT_NAME_DFLT = "Root";

private:
    QVector<Item> items;
    int maxTailSize;

    static const int MAX_TAIL_SIZE_DFLT = 3;
    static const char* PATH_DELIMITER;

public:
    MDBreadCrumbPrivate(int tailSize = MAX_TAIL_SIZE_DFLT);

    void addItem(const QString& key, const QString& value);
    QString getKey(int index) const;
    QString getValue(int index) const;
    void removeAll();
    void removeItem(int index);
    void removeLast();
    int size() const;
    bool isEmpty() const;
    bool hasCollapsedGroup() const;

    QString getRootKey() const;
    QStringList getCollapsedGroup() const;
    QStringList getTail() const;
    int getMaxTailSize() const;

};

}

class MDBreadCrumb : public QListWidget {
    Q_OBJECT

public:
    using Ptr = MDBreadCrumb*;
    using ConstPtr = const MDBreadCrumb*;

private:
    internal::MDBreadCrumbPrivate* dPtr;
    QList<QMetaObject::Connection> connections;
    QString rootName;
    QString currentPath;
    QDir rootPath;

    static const char* DOTS;
    static const char* DEFAULT_ROOT_NAME;

public:
    MDBreadCrumb(QWidget *parent = Q_NULLPTR);
    void updatePath(const QString& path);
    void setRoot(const QString& rootName, const QString& rootPath);
    void setRoot(const QString& rootName, const QDir& rootPath);
    QString getRootName() const;

private:
    void addItem(const QString& item, const QString& path);
    void removeLast();
    void setupSignalsAndSlots();
    void setupButtonsSignalsAndSlots();
    void setupUi();
    void updateButtonsUi();

private slots:
    void manageClickEvent(int index);

signals:
    void itemClicked(const QString& item);
    void pathClicked(const QString& path);

};

#endif // MDBREADCRUMB_HPP
