#include "MDFileExplorerWidgetLogic.hpp"
#include "MDFileExplorerWidget.hpp"

/*************************************************************
 *    M D  F I L E  E X P L O R E R  W I D G E T  L O G I C
 *************************************************************/

MDFileExplorerWidgetLogic::MDFileExplorerWidgetLogic(const QString& rootPath) {

    if (!rootPath.isEmpty())
        setPath(rootPath);

}

void MDFileExplorerWidgetLogic::setPath(const QString& path) {
    model.setRootPath(path);
}

QString MDFileExplorerWidgetLogic::currentPath() const {
    return model.rootPath();
}



/****************************************************
 *    M D  F I L E  E X P L O R E R  W I D G E T
 ****************************************************/

MDFileExplorerWidget::MDFileExplorerWidget(QWidget* parent) :
    QListView(parent), dPtr(new MDFileExplorerWidgetLogic()) {

    dPtr->qPtr = this;
    this->setModel(&dPtr->model);
    this->setupSignalsAndSlots();

}

MDFileExplorerWidget::~MDFileExplorerWidget() {
    delete dPtr;
}

void MDFileExplorerWidget::setPath(const QString& path) {

    QDir test(path);
    if (!test.exists())
        return;

    dPtr->setPath(path);
    QModelIndex index = dPtr->model.index(path);
    this->setRootIndex(index);
    emit currentSubFolderSignal(test.fromNativeSeparators(test.absolutePath()));

}

void MDFileExplorerWidget::setupSignalsAndSlots() {

    connect(this, &MDFileExplorerWidget::clicked, [&](const QModelIndex &index) {

        if (dPtr->model.isDir(index)) {
            QString dirPath = this->dPtr->model.filePath(index);
//            QString dirName = this->dPtr->model.fileName(index);
            this->setPath(dirPath);
            emit currentSubFolderSignal(dirPath);
        }

    });

}
