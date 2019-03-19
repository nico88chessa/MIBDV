#include "FileExplorerFrame.hpp"
#include "ui_FileExplorerFrame.h"

#include <QDir>

FileExplorerFrame::FileExplorerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FileExplorerFrame) {

    ui->setupUi(this);

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

    static int cont = 1;

    connect(ui->lvExplorer, &MDFileExplorerWidget::currentFolderSignal, this, &FileExplorerFrame::updateBreadCrumb);
    connect(ui->lvExplorer, &MDFileExplorerWidget::currentItemSignal, ui->idwTest, &ItemDetailWidget::setCurrentPath);
    connect(ui->lvExplorer, &MDFileExplorerWidget::itemsPathSignal, ui->idwTest, &ItemDetailWidget::updateModelData);

    connect(ui->breadCrumb, &MDBreadCrumb::pathClicked, this, &FileExplorerFrame::updateFileExplorer);

}

void FileExplorerFrame::updateBreadCrumb(const QString& folderPath) {
    this->ui->breadCrumb->updatePath(folderPath);
}

void FileExplorerFrame::updateFileExplorer(const QString& folderPath) {
    this->ui->lvExplorer->setPath(folderPath);
}
