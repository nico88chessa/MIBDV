#include "FileExplorerFrame.hpp"
#include "ui_FileExplorerFrame.h"

#include <QDir>

FileExplorerFrame::FileExplorerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FileExplorerFrame) {

    ui->setupUi(this);

    QString rootPath = "C://Users//nicola//workspace";

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

//    connect(ui->pbAdd, &QPushButton::clicked, [&]{
//        this->ui->breadCrumb->addItem(QString("prova - %1").arg(cont++), "test");
//    });
//    connect(ui->pbRemove, &QPushButton::clicked, [&]{
//        this->ui->breadCrumb->removeLast();
//    });
    connect(ui->lvExplorer, &MDFileExplorerWidget::currentSubFolderSignal, this, &FileExplorerFrame::updateBreadCrumb);
    connect(ui->breadCrumb, &MDBreadCrumb::pathClicked, this, &FileExplorerFrame::updateFileExplorer);

}

void FileExplorerFrame::updateBreadCrumb(const QString& folderPath) {

    // qui devo verificare se sulla breadcrumb devo aggiungere o rimuovere degli item
    this->ui->breadCrumb->updatePath(folderPath);
}

void FileExplorerFrame::updateFileExplorer(const QString& folderPath) {
    this->ui->lvExplorer->setPath(folderPath);
}
