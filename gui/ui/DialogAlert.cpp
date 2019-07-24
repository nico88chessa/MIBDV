#include "DialogAlert.hpp"
#include "ui_DialogAlert.h"

#include <Logger.hpp>


using namespace PROGRAM_NAMESPACE;

DialogAlert::DialogAlert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAlert) {

    this->setupUi();

    this->setupSignalsAndSlots();

}

DialogAlert::~DialogAlert() {
    delete ui;
}

void DialogAlert::setupUi() {

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());
    this->setWindowModality(Qt::ApplicationModal);
    this->setModal(true);

}

void DialogAlert::setupLabels(const QString& title, const QString& content) {

    traceEnter;

    ui->lTitle->setText(title);
    ui->lContent->setText(content);

    traceExit;

}

void DialogAlert::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->pbCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->pbOk, &QPushButton::clicked, this, &QDialog::accept);

    traceExit;

}
