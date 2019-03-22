#include "DialogRename.hpp"
#include "ui_DialogRename.h"

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

DialogRename::DialogRename(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRename) {

    this->setupUi();

    this->setupSignalsAndSlots();

}

DialogRename::~DialogRename() {
    delete ui;
}

void DialogRename::setupUi() {

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());

    this->setModal(true);

}

void DialogRename::setupFilename(const QString& filename) {

    traceEnter;

    ui->leNewFilename->setText(filename);

    traceExit;

}

void DialogRename::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->pbCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->pbOk, &QPushButton::clicked, this, &QDialog::accept);

    traceExit;

}

QString DialogRename::getFilename() const {

    traceEnter;
    QString filename = this->ui->leNewFilename->text();
    return filename;
    traceExit;

}
