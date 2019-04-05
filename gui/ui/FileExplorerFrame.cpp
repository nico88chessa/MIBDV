#include "FileExplorerFrame.hpp"
#include "ui_FileExplorerFrame.h"

#include <QDir>
#include <Settings.hpp>
#include <Logger.hpp>


using namespace PROGRAM_NAMESPACE;

FileExplorerFrame::FileExplorerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FileExplorerFrame) {

    this->setupUi();

    Settings& settings = Settings::instance();

    traceEnter;

    traceInfo() << "Spool path:" << settings.getUiSpoolPath();

    this->ui->breadCrumb->setRoot("/", settings.getUiSpoolPath());
    this->ui->breadCrumb->updatePath(settings.getUiSpoolPath());
    ui->lvExplorer->setPath(settings.getUiSpoolPath());

    this->setupSignalsAndSlots();

    traceExit;

}

FileExplorerFrame::~FileExplorerFrame() {
    traceEnter;
    delete ui;
    traceExit;
}

void FileExplorerFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->lvExplorer, &MDFileExplorerWidget::currentFolderSignal, this, &FileExplorerFrame::updateBreadCrumb);
    connect(ui->lvExplorer, &MDFileExplorerWidget::currentFolderSignal, [&]() {
        ui->itwDetail->setCurrentPath("");
    });
    connect(ui->lvExplorer, &MDFileExplorerWidget::currentItemSignal, [&](const QString& filePath) {
        this->ui->pbDelete->setEnabled(true);
        this->ui->pbRename->setEnabled(true);
        this->ui->itwDetail->setCurrentPath(filePath);
    });
    connect(ui->lvExplorer, &MDFileExplorerWidget::itemsPathSignal, ui->itwDetail, &ItemDetailWidget::updateModelData);
    connect(ui->lvExplorer, &MDFileExplorerWidget::selectionClearSignal, [&]() {
        ui->itwDetail->setCurrentPath("");
        this->ui->pbDelete->setEnabled(false);
        this->ui->pbRename->setEnabled(false);
    });

    connect(ui->breadCrumb, &MDBreadCrumb::pathClicked, this, &FileExplorerFrame::updateFileExplorer);

    connect(ui->pbDelete, &QPushButton::clicked, ui->lvExplorer, &MDFileExplorerWidget::removeCurrentItem);
    connect(ui->pbRename, &QPushButton::clicked, ui->lvExplorer, &MDFileExplorerWidget::renameCurrentItem);

    traceExit;

}

void FileExplorerFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);
    this->ui->pbDelete->setEnabled(false);
    this->ui->pbRename->setEnabled(false);

    traceExit;

}

void FileExplorerFrame::updateBreadCrumb(const QString& folderPath) {
    this->ui->breadCrumb->updatePath(folderPath);
}

void FileExplorerFrame::updateFileExplorer(const QString& folderPath) {
    this->ui->lvExplorer->setPath(folderPath);
}
