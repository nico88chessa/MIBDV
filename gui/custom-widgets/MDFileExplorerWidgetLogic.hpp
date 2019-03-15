#ifndef MDFILEEXPLORERWIDGETLOGIC_HPP
#define MDFILEEXPLORERWIDGETLOGIC_HPP

#include <QString>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>


/*************************************************************
 *    M O D E L  V A L I D A T O R  F I L T E R
 *************************************************************/

class ModelValidatorFilter : public QSortFilterProxyModel {
    Q_OBJECT

public:
    using Ptr = ModelValidatorFilter*;
    using ConstPtr = const ModelValidatorFilter*;

public:
    explicit ModelValidatorFilter(QObject *parent = Q_NULLPTR);
    ~ModelValidatorFilter();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
};


/**************************************************************
 *    M D  F I L E  E X P L O R E R  W I D G E T  L O G I C
 **************************************************************/

class MDFileExplorerWidget;

class MDFileExplorerWidgetLogic {
public:
    using Ptr = MDFileExplorerWidgetLogic*;
    using ConstPtr = const MDFileExplorerWidgetLogic*;

    friend class MDFileExplorerWidget;

private:
    MDFileExplorerWidget* qPtr;
    QFileSystemModel* model;
    ModelValidatorFilter* filter;

public:
    MDFileExplorerWidgetLogic(const QString& rootPath = "");
    ~MDFileExplorerWidgetLogic();
    void setPath(const QString& path);
    QString currentPath() const;

protected:
    bool isDir(const QModelIndex& index) const;
    QString getPath(const QModelIndex& index);
    QModelIndex getIndex(const QString& path) const;
    inline QAbstractItemModel* getModel();

};

#endif // MDFILEEXPLORERWIDGETLOGIC_HPP
