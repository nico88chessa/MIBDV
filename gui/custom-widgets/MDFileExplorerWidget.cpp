#include "MDFileExplorerWidgetLogic.hpp"
#include "MDFileExplorerWidget.hpp"

#include <QKeyEvent>
#include <QEvent>
#include <QGestureEvent>
#include <Logger.hpp>
#include <gui/ui/DialogAlert.hpp>
#include <gui/ui/DialogRename.hpp>


/*
 *    M O D E L   V A L I D A T O R   F I L T E R
 */

ModelValidatorFilter::ModelValidatorFilter(QObject *parent) :
    QSortFilterProxyModel(parent) { }

ModelValidatorFilter::~ModelValidatorFilter() { }

bool ModelValidatorFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {

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


/*
 *    M D   F I L E   E X P L O R E R   W I D G E T   L O G I C
 */

MDFileExplorerWidgetLogic::MDFileExplorerWidgetLogic(const QString& rootPath) :
    model(Q_NULLPTR), filter(Q_NULLPTR) {

    this->model = new QFileSystemModel();
    this->filter = new ModelValidatorFilter();
    this->filter->setSourceModel(model);
    if (!rootPath.isEmpty())
        setPath(rootPath);

}

MDFileExplorerWidgetLogic::~MDFileExplorerWidgetLogic() {

    delete this->model;
    delete this->filter;

}

void MDFileExplorerWidgetLogic::setPath(const QString& path) {
    model->setRootPath(path);
}

QString MDFileExplorerWidgetLogic::currentPath() const {
    return model->rootPath();
}

bool MDFileExplorerWidgetLogic::isDir(const QModelIndex& index) const {

    QModelIndex modelIndex = filter->mapToSource(index);
    if (model->isDir(modelIndex))
        return true;
    return false;

}

QString MDFileExplorerWidgetLogic::getPath(const QModelIndex& index) const {

    QModelIndex sourceModelIndex = filter->mapToSource(index);
    return this->model->filePath(sourceModelIndex);

}

QString MDFileExplorerWidgetLogic::getFilename(const QModelIndex& index) const {

    QModelIndex sourceModelIndex = filter->mapToSource(index);
    return this->model->fileName(sourceModelIndex);

}

QFileInfo MDFileExplorerWidgetLogic::getFileInfo(const QModelIndex& index) const {

    QModelIndex sourceModelIndex = filter->mapToSource(index);
    return this->model->fileInfo(sourceModelIndex);

}

QModelIndex MDFileExplorerWidgetLogic::getIndex(const QString& path) const {
    QModelIndex index = filter->mapFromSource(model->index(path));
    return index;
}

QAbstractItemModel* MDFileExplorerWidgetLogic::getModel() {
    return filter;
}


/*
 *    M D   F I L E   E X P L O R E R   W I D G E T
 */

MDFileExplorerWidget::MDFileExplorerWidget(QWidget* parent) :
    QListView(parent), dPtr(new MDFileExplorerWidgetLogic()),
    isTapAndHoldEventInProgress(false) {

    dPtr->qPtr = this;
    this->setModel(dPtr->filter);
    this->setupSignalsAndSlots();
    this->grabGesture(Qt::TapAndHoldGesture);

}

MDFileExplorerWidget::~MDFileExplorerWidget() {
    delete dPtr;
}

void MDFileExplorerWidget::setPath(const QString& path) {

    QDir test(path);
    if (!test.exists())
        return;

    dPtr->setPath(path);
    QModelIndex index = dPtr->getIndex(path);
    this->setRootIndex(index);
    emit currentFolderSignal(test.fromNativeSeparators(test.absolutePath()));

}

void MDFileExplorerWidget::clearSelection() {

    QListView::clearSelection();
    emit selectionClearSignal();

}

void MDFileExplorerWidget::removeCurrentItem() {

    QModelIndex index = this->currentIndex();
    QFileInfo fileInfo = dPtr->getFileInfo(index);

    QScopedPointer<DialogAlert> dialog(new DialogAlert);
    // TODO NIC 22/03/2019 - spostare stringhe in costanti traducibili
    dialog->setupLabels("Cancellazione file", QString("Confermare cancellazione file %1?").arg(fileInfo.fileName()));

    if (dialog->exec() == QDialog::Accepted) {
        this->clearSelection();
        dPtr->model->remove(dPtr->filter->mapToSource(index));
    }

}

void MDFileExplorerWidget::renameCurrentItem() {

    QModelIndex index = this->currentIndex();
    QFileInfo fileInfo = dPtr->getFileInfo(index);

    QScopedPointer<DialogRename> dialog(new DialogRename);
    dialog->setupFilename(fileInfo.fileName());

    if (dialog->exec() == QDialog::Accepted) {

        if (fileInfo.isDir()) {

            QDir d(fileInfo.absoluteDir());
            if (!d.rename(fileInfo.fileName(), dialog->getFilename())) {
                QScopedPointer<DialogAlert> dialog(new DialogAlert);
                // TODO NIC 22/03/2019 - spostare stringhe in costanti traducibili
                dialog->setupLabels("Error", "Impossibile rinominare il file");
                dialog->exec();
            }

        } else {

            QString renameFile = fileInfo.absolutePath();
            renameFile.append("/").append(dialog->getFilename());
            QFile f(fileInfo.absoluteFilePath());
            if (!f.rename(renameFile)) {
                QScopedPointer<DialogAlert> dialog(new DialogAlert);
                // TODO NIC 22/03/2019 - spostare stringhe in costanti traducibili
                dialog->setupLabels("Error", "Impossibile rinominare il file");
                dialog->exec();
            }

        }

    }

}

void MDFileExplorerWidget::setupSignalsAndSlots() {

    connect(this, &MDFileExplorerWidget::clicked, [&](const QModelIndex &index) {

        /* NOTE NIC 22/03/2019 - controllo che il click non sia a seguito dell'evento Tap and Hold;
         * se si, non faccio nulla, in quanto all'interno dell'evento stesso genero il segnale currentItemSignal(..)
         */
        if (!this->isTapAndHoldEventInProgress) {

            QString path = this->dPtr->getPath(index);

            if (dPtr->isDir(index)) {
                this->setPath(path);
                emit currentFolderSignal(path);
            } else
                emit currentItemSignal(path);

        }

    });

    connect(this->dPtr->model, &QFileSystemModel::directoryLoaded, [&](const QString& path) {

        QModelIndex sourceModelIndex = this->dPtr->model->index(path);
        QModelIndex filterModelIndex = this->dPtr->filter->mapFromSource(sourceModelIndex);
        int rowCount = this->dPtr->filter->rowCount(filterModelIndex);

        QStringList itemsPath;

        for (int i=0; i<rowCount; ++i) {
            QModelIndex currentfilterModelIndex = this->dPtr->filter->index(i, 0, filterModelIndex);
            QModelIndex currentSourceModelIndex = this->dPtr->filter->mapToSource(currentfilterModelIndex);
            QFileInfo fileInfo = this->dPtr->model->fileInfo(currentSourceModelIndex);
            itemsPath.append(fileInfo.absoluteFilePath());
        }

        this->clearSelection();

        if (!itemsPath.isEmpty())
            emit itemsPathSignal(itemsPath);

    });

}

bool MDFileExplorerWidget::event(QEvent* event) {

    using namespace PROGRAM_NAMESPACE;

    traceDebug() << "Event: " << event->type();

    if (event->type() == QEvent::Gesture) {

        // NOTE NIC 18/03/2019 - alla fine di questo evento, viene generato l'evento MouseRelease
        // che (guardare codice Qt) genera il segnale emit clicked();

        QGestureEvent* gestureEvent = static_cast<QGestureEvent*>(event);
        if (QGesture* gesture = gestureEvent->gesture(Qt::TapAndHoldGesture)) {
            isTapAndHoldEventInProgress = true;
            QModelIndex newIdx = currentIndex();
            QString itemPath = this->dPtr->getPath(newIdx);
            emit currentItemSignal(itemPath);
            return true;
        }

    } else if (event->type() == QEvent::KeyPress) {

        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        QModelIndex oldIdx = currentIndex();
        QListView::event(event);
        QModelIndex newIdx = currentIndex();

        if (oldIdx.row() != newIdx.row()) {
            QString itemPath = this->dPtr->getPath(newIdx);
            emit currentItemSignal(itemPath);
        }
        return true;

    }

    return QListView::event(event);

}

void MDFileExplorerWidget::mousePressEvent(QMouseEvent* event) {

    // NOTE NIC 19/03/2018 - al click del mouse, resetto il flag per capire
    // se l'azione che sto facendo e' un singolo click oppure

    isTapAndHoldEventInProgress = false;
    QListView::mousePressEvent(event);
}
