#include "MDFileExplorerWidget.hpp"

#include <QKeyEvent>
#include <QEvent>
#include <QGestureEvent>
#include <Logger.hpp>
#include <gui/ui/DialogAlert.hpp>
#include <gui/ui/DialogRename.hpp>


/*
 *    M D   F I L E   E X P L O R E R   W I D G E T
 */

MDFileExplorerWidget::MDFileExplorerWidget(QWidget* parent) :
    QListView(parent),
    isTapAndHoldEventInProgress(false),
    model(new FilterFileExplorerModel) {

    this->setModel(model);
    this->setupSignalsAndSlots();
    this->grabGesture(Qt::TapAndHoldGesture);

    /* TODO NIC 25/03/2019 - multi selezione
     * implementare multi selezione e rimozione di item
     * questo richiedera' lo spostamento del model alla class
     * FileExplorerFrame
     */
    //    this->setSelectionMode(SelectionMode::ExtendedSelection);

}

MDFileExplorerWidget::~MDFileExplorerWidget() {
    delete model;
}

void MDFileExplorerWidget::setPath(const QString& path) {

    QDir test(path);
    if (!test.exists())
        return;

    /* TODO NIC 25/03/2019 - set root path va chiamato una volta sola
     * da rivedere la logica con la multiselezione
     */
    model->setRootPath(path);
    QModelIndex index = model->getIndex(path);
    this->setRootIndex(index);
    emit currentFolderSignal(test.fromNativeSeparators(test.absolutePath()));

}

void MDFileExplorerWidget::clearSelection() {

    QListView::clearSelection();
    emit selectionClearSignal();

}

void MDFileExplorerWidget::removeCurrentItem() {

    QModelIndex index = this->currentIndex();
    QFileInfo fileInfo = model->getFileInfo(index);

    QScopedPointer<DialogAlert> dialog(new DialogAlert);
    // TODO NIC 22/03/2019 - spostare stringhe in costanti traducibili
    dialog->setupLabels("Cancellazione file", QString("Confermare cancellazione file %1?").arg(fileInfo.fileName()));

    if (dialog->exec() == QDialog::Accepted) {
        this->clearSelection();
        model->remove(index);
    }

}

void MDFileExplorerWidget::renameCurrentItem() {

    QFileInfo fileInfo = model->getFileInfo(this->currentIndex());

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

            QString path = model->getPath(index);

            if (model->isDir(index)) {
                this->setPath(path);
                emit currentFolderSignal(path);
            } else
                emit currentItemSignal(path);

        }

    });

    connect(this->model, &FilterFileExplorerModel::directoryLoaded, [&](const QString& path) {

        QModelIndex index = model->getIndex(path);
        int rowCount = model->rowCount(index);

        QStringList itemsPath;

        for (int i=0; i<rowCount; ++i) {
            QModelIndex currentIndex = model->index(i, 0, index);
            QFileInfo fileInfo = this->model->getFileInfo(currentIndex);
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
            QString itemPath = this->model->getPath(newIdx);
            emit currentItemSignal(itemPath);
            return true;
        }

    } else if (event->type() == QEvent::KeyPress) {

        QModelIndex oldIdx = currentIndex();
        QListView::event(event);
        QModelIndex newIdx = currentIndex();

        if (oldIdx.row() != newIdx.row()) {
            QString itemPath = this->model->getPath(newIdx);
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
