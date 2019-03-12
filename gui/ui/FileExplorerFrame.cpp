#include "FileExplorerFrame.hpp"
#include "ui_FileExplorerFrame.h"

FileExplorerFrame::FileExplorerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FileExplorerFrame)
{
    ui->setupUi(this);

    static int cont = 1;
    connect(ui->pbAdd, &QPushButton::clicked, [&]{
        this->ui->breadCrumb->addItem(QString("prova - prova - %1").arg(cont++));
    });
    connect(ui->pbRemove, &QPushButton::clicked, [&]{
        this->ui->breadCrumb->removeLast();
    });
}

FileExplorerFrame::~FileExplorerFrame()
{
    delete ui;
}
