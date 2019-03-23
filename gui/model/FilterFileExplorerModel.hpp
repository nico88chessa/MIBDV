#ifndef FILTERFILEEXPLORERMODEL_HPP
#define FILTERFILEEXPLORERMODEL_HPP

#include <QFileSystemModel>
#include <QSortFilterProxyModel>


/**
 * @brief The FilterFileExplorerModel class
 */
class FilterFileExplorerModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    using Ptr = FilterFileExplorerModel*;
    using ConstPtr = const FilterFileExplorerModel*;

private:
    QScopedPointer<QFileSystemModel> fsModel;

public:
    explicit FilterFileExplorerModel(QObject *parent = Q_NULLPTR);
    ~FilterFileExplorerModel();

    void setRootPath(const QString& path);
    QString currentPath() const;
    bool isDir(const QModelIndex& index) const;
    QString getPath(const QModelIndex& index) const;
    QString getFilename(const QModelIndex& index) const;
    QFileInfo getFileInfo(const QModelIndex& index) const;
    QModelIndex getIndex(const QString& path) const;
    bool remove(const QModelIndex &index);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

signals:
    void rootPathChanged(const QString &newPath);
    void fileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void directoryLoaded(const QString &path);

};
#endif // FILTERFILEEXPLORERMODEL_HPP
