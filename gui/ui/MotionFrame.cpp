#include "MotionFrame.hpp"
#include "ui_MotionFrame.h"

#include "MotionFrameLogic.hpp"

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

/**********************************************
 *    M O T I O N  F R A M E  L O G I C
 *********************************************/

MotionFrameLogic::MotionFrameLogic() {

}

MotionFrameLogic::~MotionFrameLogic() { }

void MotionFrameLogic::setupLogic(const QSharedPointer<MotionManager>& motionManager) {

    traceEnter;
    this->motionManager = motionManager;
    traceExit;

}

void MotionFrameLogic::moveAxisX() {

    traceEnter;

    QString positionStr = qPtr->ui->leAxisXPosition->text();
    double position = positionStr.toDouble();

    motionManager->moveX(position);

    traceExit;

}


/**********************************************
 *         M O T I O N  F R A M E
 *********************************************/

void MotionFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->pbMoveX, &QPushButton::clicked, dPtr, &MotionFrameLogic::moveAxisX);

    traceExit;

}

void MotionFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    Settings& s = Settings::instance();

    ui->leAxisXMaxPosition->setText(QString::number(s.getAxisXMaxPosMm(), 'g', 3));
    ui->leAxisYMaxPosition->setText(QString::number(s.getAxisYMaxPosMm(), 'g', 3));
    ui->leAxisZMaxPosition->setText(QString::number(s.getAxisZMaxPosMm(), 'g', 3));

    traceExit;

}

MotionFrame::MotionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MotionFrame), dPtr(new MotionFrameLogic()) {

    dPtr->qPtr = this;

    this->setupUi();

    this->setupSignalsAndSlots();

}

MotionFrame::~MotionFrame() {
    delete ui;
}

void MotionFrame::setupLogic(const QSharedPointer<MotionManager>& motionManager) {

    traceEnter;

    dPtr->setupLogic(motionManager);

    traceExit;

}

void MotionFrame::updateUI(const MotionBean& bean) {

    traceEnter;

    ui->leAxisXCurrentPosition->setText(QString::number(bean.getAxisXPosition(), 'f', 3));
    ui->leAxisYCurrentPosition->setText(QString::number(bean.getAxisYPosition(), 'f', 3));
    ui->leAxisZCurrentPosition->setText(QString::number(bean.getAxisZPosition(), 'f', 3));

    ui->cbAxisXForwardLimit->setChecked(bean.getAxisXForwardLimit());
    ui->cbAxisXMotorOff->setChecked(bean.getAxisXMotorOff());
    ui->cbAxisXMoving->setChecked(bean.getAxisXMoveInProgress());
    ui->cbAxisXReverseLimit->setChecked(bean.getAxisXReverseLimit());

    ui->cbAxisYForwardLimit->setChecked(bean.getAxisYForwardLimit());
    ui->cbAxisYMotorOff->setChecked(bean.getAxisYMotorOff());
    ui->cbAxisYMoving->setChecked(bean.getAxisYMoveInProgress());
    ui->cbAxisYReverseLimit->setChecked(bean.getAxisYReverseLimit());

    ui->cbAxisZForwardLimit->setChecked(bean.getAxisZForwardLimit());
    ui->cbAxisZMotorOff->setChecked(bean.getAxisZMotorOff());
    ui->cbAxisZMoving->setChecked(bean.getAxisZMoveInProgress());
    ui->cbAxisZReverseLimit->setChecked(bean.getAxisZReverseLimit());

    traceExit;

}

