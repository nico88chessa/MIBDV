#include "MotionFrame.hpp"
#include "ui_MotionFrame.h"

MotionFrame::MotionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MotionFrame)
{
    ui->setupUi(this);
}

MotionFrame::~MotionFrame()
{
    delete ui;
}
