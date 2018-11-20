#ifndef MOTIONBEAN_HPP
#define MOTIONBEAN_HPP

#include <Constants.hpp>
#include <Settings.hpp>

#include <GalilCNStatusBean.hpp>

namespace PROGRAM_NAMESPACE {

class MotionBean {
public:
    using Ptr = MotionBean*;
    using ConstPtr = const MotionBean*;

private:

    int axisXStepPerMm;
    int axisYStepPerMm;
    int axisZStepPerMm;

    bool axisXForwardLimit;
    bool axisXReverseLimit;
    bool axisXMoveInProgress;
    bool axisXHomeInput;
    bool axisXMotorOff;
    real axisXPosition;

    bool axisYForwardLimit;
    bool axisYReverseLimit;
    bool axisYHomeInput;
    bool axisYMoveInProgress;
    bool axisYMotorOff;
    real axisYPosition;

    bool axisZForwardLimit;
    bool axisZReverseLimit;
    bool axisZHomeInput;
    bool axisZMoveInProgress;
    bool axisZMotorOff;
    real axisZPosition;

public:

    MotionBean() {

        Settings& s = Settings::instance();

        axisXStepPerMm = s.getAxisXStepPerMm();
        axisYStepPerMm = s.getAxisYStepPerMm();
        axisZStepPerMm = s.getAxisZStepPerMm();

        axisXForwardLimit = false;
        axisXReverseLimit = false;
        axisXHomeInput = false;
        axisXMoveInProgress = false;
        axisXMotorOff = false;
        axisXPosition = 0;

        axisYForwardLimit = false;
        axisYReverseLimit = false;
        axisYHomeInput = false;
        axisYMoveInProgress = false;
        axisYMotorOff = false;
        axisYPosition = 0;

        axisZForwardLimit = false;
        axisZReverseLimit = false;
        axisZHomeInput = false;
        axisZMoveInProgress = false;
        axisZMotorOff = false;
        axisZPosition = 0;

    }

    MotionBean(const GalilCNStatusBean& galilCNStatusBean) {

        axisXForwardLimit = galilCNStatusBean.getAxisAForwardLimit();
        axisXReverseLimit = galilCNStatusBean.getAxisAReverseLimit();
        axisXMoveInProgress = galilCNStatusBean.getAxisAMoveInProgress();
        axisXHomeInput = galilCNStatusBean.getAxisAHomeInput();
        axisXMotorOff = galilCNStatusBean.getAxisAMotorOff();
        axisXPosition = galilCNStatusBean.getAxisAMotorPosition() * axisXStepPerMm;

        axisYForwardLimit = galilCNStatusBean.getAxisBForwardLimit();
        axisYReverseLimit = galilCNStatusBean.getAxisBReverseLimit();
        axisYMoveInProgress = galilCNStatusBean.getAxisBMoveInProgress();
        axisYHomeInput = galilCNStatusBean.getAxisBHomeInput();
        axisYMotorOff = galilCNStatusBean.getAxisBMotorOff();
        axisYPosition = galilCNStatusBean.getAxisBMotorPosition() * axisYStepPerMm;

        axisZForwardLimit = galilCNStatusBean.getAxisCForwardLimit();
        axisZReverseLimit = galilCNStatusBean.getAxisCReverseLimit();
        axisZMoveInProgress = galilCNStatusBean.getAxisCMoveInProgress();
        axisZHomeInput = galilCNStatusBean.getAxisCHomeInput();
        axisZMotorOff = galilCNStatusBean.getAxisCMotorOff();
        axisZPosition = galilCNStatusBean.getAxisCMotorPosition() * axisZStepPerMm;

    }

    bool getAxisXReverseLimit() const { return axisXReverseLimit; }
    void setAxisXReverseLimit(bool value) { axisXReverseLimit = value; }
    bool getAxisXHomeInput() const { return axisXHomeInput; }
    void setAxisXHomeInput(bool value) { axisXHomeInput = value; }
    bool getAxisXMotorOff() const { return axisXMotorOff; }
    void setAxisXMotorOff(bool value) { axisXMotorOff = value; }
    real getAxisXPosition() const { return axisXPosition; }
    void setAxisXPosition(const real& value) { axisXPosition = value; }
    bool getAxisYForwardLimit() const { return axisYForwardLimit; }
    void setAxisYForwardLimit(bool value) { axisYForwardLimit = value; }
    bool getAxisYReverseLimit() const { return axisYReverseLimit; }
    void setAxisYReverseLimit(bool value) { axisYReverseLimit = value; }
    bool getAxisYHomeInput() const { return axisYHomeInput; }
    void setAxisYHomeInput(bool value) { axisYHomeInput = value; }
    bool getAxisYMoveInProgress() const { return axisYMoveInProgress; }
    void setAxisYMoveInProgress(bool value) { axisYMoveInProgress = value; }
    bool getAxisYMotorOff() const { return axisYMotorOff; }
    void setAxisYMotorOff(bool value) { axisYMotorOff = value; }
    real getAxisYPosition() const { return axisYPosition; }
    void setAxisYPosition(const real& value) { axisYPosition = value; }
    bool getAxisZForwardLimit() const { return axisZForwardLimit; }
    void setAxisZForwardLimit(bool value) { axisZForwardLimit = value; }
    bool getAxisZReverseLimit() const { return axisZReverseLimit; }
    void setAxisZReverseLimit(bool value) { axisZReverseLimit = value; }
    bool getAxisZHomeInput() const { return axisZHomeInput; }
    void setAxisZHomeInput(bool value) { axisZHomeInput = value; }
    bool getAxisZMoveInProgress() const { return axisZMoveInProgress; }
    void setAxisZMoveInProgress(bool value) { axisZMoveInProgress = value; }
    bool getAxisZMotorOff() const { return axisZMotorOff; }
    void setAxisZMotorOff(bool value) { axisZMotorOff = value; }
    real getAxisZPosition() const { return axisZPosition; }
    void setAxisZPosition(const real& value) { axisZPosition = value; }
    bool getAxisXForwardLimit() const { return axisXForwardLimit; }
    void setAxisXForwardLimit(bool value) { axisXForwardLimit = value; }

};


}

#endif // MOTIONBEAN_HPP
