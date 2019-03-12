#ifndef MDBREADCRUMB_HPP
#define MDBREADCRUMB_HPP

#include <QListWidget>
#include <QStringList>
#include <QVector>

namespace internal {

class MDBreadCrumbPrivate {
public:
    using Ptr = MDBreadCrumbPrivate*;
    using ConstPtr = const MDBreadCrumbPrivate*;

public:
    static constexpr char* ROOT_NAME_DFLT = "Root";

private:
    struct Item {
    public:
        QString itemKey; // singola stringa
        QStringList itemValue; // prefisso contenente tutte le stringhe 'genitori'
    };

    QVector<Item> items;
    int maxTailSize;

    static const int MAX_TAIL_SIZE_DFLT = 3;
    static const char* PATH_DELIMITER;

public:
    MDBreadCrumbPrivate(const QString& rootName = ROOT_NAME_DFLT,
                        int tailSize = MAX_TAIL_SIZE_DFLT);

    void addItem(const QString& item);
    QStringList getItem(int index) const;
    QString getKey(int index) const;
    QString getPath(int index) const;
    void removeItem(int index);
    void removeLast();
    int size() const;
    bool hasCollapsedGroup() const;

    QString getRoot() const;
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

    static const char* DOTS;

public:
    MDBreadCrumb(QWidget *parent = Q_NULLPTR);
    void addItem(const QString& item);
    void removeLast();

private:
    void setupSignalsAndSlots();
    void setupUi();
    void updateButtonsUi();

private slots:
    void manageClickEvent(int index);
//    void manageClickEvent(QListWidgetItem* item);

signals:
    void itemClicked(const QString& item);

};

#endif // MDBREADCRUMB_HPP
