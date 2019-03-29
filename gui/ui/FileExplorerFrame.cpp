#include "FileExplorerFrame.hpp"
#include "ui_FileExplorerFrame.h"

#include <QDir>

FileExplorerFrame::FileExplorerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FileExplorerFrame),
    currentItemPath() {

    this->setupUi();

    QString rootPath = "C:/Users/nicola/Desktop/Spool";

    this->ui->breadCrumb->setRoot("/", rootPath);
    this->ui->breadCrumb->updatePath(rootPath);
    ui->lvExplorer->setPath(rootPath);

    this->setupSignalsAndSlots();

}

FileExplorerFrame::~FileExplorerFrame() {
    delete ui;
}

void FileExplorerFrame::setupSignalsAndSlots() {

    connect(ui->lvExplorer, &MDFileExplorerWidget::currentFolderSignal, this, &FileExplorerFrame::updateBreadCrumb);
    connect(ui->lvExplorer, &MDFileExplorerWidget::currentFolderSignal, [&]() {
        ui->itwDetail->setCurrentPath("");
    });
    connect(ui->lvExplorer, &MDFileExplorerWidget::currentItemSignal, [&](const QString& filePath) {
        this->ui->pbDelete->setEnabled(true);
        this->ui->pbRename->setEnabled(true);
        this->ui->pbSetFile->setEnabled(true);
        this->ui->itwDetail->setCurrentPath(filePath);
        currentItemPath = filePath;
    });

    connect(ui->lvExplorer, &MDFileExplorerWidget::itemsPathSignal, ui->itwDetail, &ItemDetailWidget::updateModelData);
    connect(ui->lvExplorer, &MDFileExplorerWidget::selectionClearSignal, [&]() {
        ui->itwDetail->setCurrentPath("");
        this->ui->pbDelete->setEnabled(false);
        this->ui->pbRename->setEnabled(false);
        this->ui->pbSetFile->setEnabled(false);
    });

    connect(ui->breadCrumb, &MDBreadCrumb::pathClicked, this, &FileExplorerFrame::updateFileExplorer);

    connect(ui->pbDelete, &QPushButton::clicked, ui->lvExplorer, &MDFileExplorerWidget::removeCurrentItem);
    connect(ui->pbRename, &QPushButton::clicked, ui->lvExplorer, &MDFileExplorerWidget::renameCurrentItem);

    connect(ui->pbSetFile, &QPushButton::clicked, [&]() {
        emit this->currentFileSignal(this->currentItemPath);
    });

}

void FileExplorerFrame::setupUi() {

    ui->setupUi(this);
    this->ui->pbDelete->setEnabled(false);
    this->ui->pbRename->setEnabled(false);

}

void FileExplorerFrame::updateBreadCrumb(const QString& folderPath) {
    this->ui->breadCrumb->updatePath(folderPath);
}

void FileExplorerFrame::updateFileExplorer(const QString& folderPath) {
    this->ui->lvExplorer->setPath(folderPath);
}
