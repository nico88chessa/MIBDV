#include "FilterFileExplorerModel.hpp"


FilterFileExplorerModel::FilterFileExplorerModel(QObject* parent) :
    QSortFilterProxyModel(parent),
    fsModel(new QFileSystemModel) {

    this->setSourceModel(this->fsModel.data());

    connect(fsModel.data(), &QFileSystemModel::rootPathChanged, this, &FilterFileExplorerModel::rootPathChanged);
    connect(fsModel.data(), &QFileSystemModel::fileRenamed, this, &FilterFileExplorerModel::fileRenamed);
    connect(fsModel.data(), &QFileSystemModel::directoryLoaded, this, &FilterFileExplorerModel::directoryLoaded);

}

FilterFileExplorerModel::~FilterFileExplorerModel() { }

void FilterFileExplorerModel::setRootPath(const QString& path) {
    this->fsModel->setRootPath(path);
}

QString FilterFileExplorerModel::currentPath() const {
    return this->fsModel->rootPath();
}

bool FilterFileExplorerModel::isDir(const QModelIndex& index) const {
    return fsModel->isDir(this->mapToSource(index));
}

QString FilterFileExplorerModel::getPath(const QModelIndex& index) const {
    QModelIndex temp2 = this->mapToSource(index);
    QString temp = fsModel->filePath(temp2);
    return temp;
}

QString FilterFileExplorerModel::getFilename(const QModelIndex& index) const {
    return fsModel->fileName(this->mapToSource(index));
}

QFileInfo FilterFileExplorerModel::getFileInfo(const QModelIndex& index) const {
    return fsModel->fileInfo(this->mapToSource(index));
}

QModelIndex FilterFileExplorerModel::getIndex(const QString& path) const {
    return this->mapFromSource(fsModel->index(path));
}

bool FilterFileExplorerModel::remove(const QModelIndex& index) {
    return fsModel->remove(this->mapToSource(index));
}

bool FilterFileExplorerModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {

    QFileSystemModel* fileSystemModel = qobject_cast<QFileSystemModel*>(this->sourceModel());
    QModelIndex index = this->sourceModel()->index(sourceRow, 0, sourceParent);

    QFileInfo info = fileSystemModel->fileInfo(index);

    if (info.isFile()) {

        // inizio i controlli

        // controllo l'estensione
        if (!(QString("json").compare(info.completeSuffix(), Qt::CaseInsensitive)==0))
            return false;

        // controllo che non inizi con la tilde
        QString fileName = info.fileName();
        if (fileName.startsWith('~'))
            return false;

        QString filePath = info.path();
        QString tempFilePath = QString("%1/~%2").arg(filePath).arg(fileName);
        QFile checkTemp(tempFilePath);
        if (checkTemp.exists())
            return false;

    }
    return true;

}
