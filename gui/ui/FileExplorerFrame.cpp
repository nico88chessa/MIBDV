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

    mapper = new QDataWidgetMapper(this);
    delegate = new ItemDetailDelegate(this);
    model = new ItemDetailModel();

    mapper->setItemDelegate(delegate);
    mapper->setModel(model);
    mapper->addMapping(this->ui->idwTest, 0);


    this->setupSignalsAndSlots();

}

FileExplorerFrame::~FileExplorerFrame() {
    delete ui;
}

void FileExplorerFrame::setupSignalsAndSlots() {

    static int cont = 1;

    connect(ui->pbAdd, &QPushButton::clicked, [&]() {

        static int count = 0;
        if (count % 2) {

            ItemDetailFolderBean bean;
            bean.setName("ciao");
            bean.setNumItems(1);
            model->addData(QVariant::fromValue(bean));

        } else {

            ItemDetailFilterBean bean;
            bean.setName("oaic");
            bean.setNumberOfPoints(count);
            bean.setMinPoint(PROGRAM_NAMESPACE::PointI(1, 1));
            bean.setMaxPoint(PROGRAM_NAMESPACE::PointI(2, 2));
            model->addData(QVariant::fromValue(bean));

        }
        ++count;

    });
    connect(ui->pbRemove, &QPushButton::clicked, [&]() {
        static int count = 0;
        if (count < mapper->model()->rowCount())
            this->mapper->setCurrentIndex(count++);
    });
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
    this->ui->breadCrumb->updatePath(folderPath);
}

void FileExplorerFrame::updateFileExplorer(const QString& folderPath) {
    this->ui->lvExplorer->setPath(folderPath);
}
