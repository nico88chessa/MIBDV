#ifndef MOTIONBEAN_HPP
#define MOTIONBEAN_HPP

#include <type_traits>

#include <Constants.hpp>
#include <Settings.hpp>
#include <GalilCNStatusBean.hpp>

namespace PROGRAM_NAMESPACE {

/**
 * NOTE NIC: sarebbe interessante controllare che il motion bean sia
 */

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

    template<typename T>
    MotionBean(const T& bean) {

        // NOTE Nic: qui ci andranno tutti i bean dei CN da implementare
        traceEnter;

        static_assert(std::is_same<T, PROGRAM_NAMESPACE::GalilCNStatusBean>::value, "Motion bean type not valid");
        Settings& s = Settings::instance();

        axisXStepPerMm = s.getAxisXStepPerMm();
        axisYStepPerMm = s.getAxisYStepPerMm();
        axisZStepPerMm = s.getAxisZStepPerMm();

        if (std::is_same<T, PROGRAM_NAMESPACE::GalilCNStatusBean>::value)
            initFromGalilCNStatusBean(bean);

        traceExit;

    }

private:

    void initFromGalilCNStatusBean(const GalilCNStatusBean& galilCNStatusBean) {

        axisXForwardLimit = galilCNStatusBean.getAxisAForwardLimit();
        axisXReverseLimit = galilCNStatusBean.getAxisAReverseLimit();
        axisXMoveInProgress = galilCNStatusBean.getAxisAMoveInProgress();
        axisXHomeInput = galilCNStatusBean.getAxisAHomeInput();
        axisXMotorOff = galilCNStatusBean.getAxisAMotorOff();
        if (!galilCNStatusBean.getAxisASMJumperInstalled())
            axisXPosition = galilCNStatusBean.getAxisAMotorPosition() / static_cast<real>(axisXStepPerMm);
        else
            axisXPosition = galilCNStatusBean.getAxisAReferencePosition() / static_cast<real>(axisXStepPerMm);

        axisYForwardLimit = galilCNStatusBean.getAxisBForwardLimit();
        axisYReverseLimit = galilCNStatusBean.getAxisBReverseLimit();
        axisYMoveInProgress = galilCNStatusBean.getAxisBMoveInProgress();
        axisYHomeInput = galilCNStatusBean.getAxisBHomeInput();
        axisYMotorOff = galilCNStatusBean.getAxisBMotorOff();
        if (!galilCNStatusBean.getAxisBSMJumperInstalled())
            axisYPosition = galilCNStatusBean.getAxisBMotorPosition() / static_cast<real>(axisYStepPerMm);
        else
            axisYPosition = galilCNStatusBean.getAxisBReferencePosition() / static_cast<real>(axisYStepPerMm);

        axisZForwardLimit = galilCNStatusBean.getAxisCForwardLimit();
        axisZReverseLimit = galilCNStatusBean.getAxisCReverseLimit();
        axisZMoveInProgress = galilCNStatusBean.getAxisCMoveInProgress();
        axisZHomeInput = galilCNStatusBean.getAxisCHomeInput();
        axisZMotorOff = galilCNStatusBean.getAxisCMotorOff();
        if (!galilCNStatusBean.getAxisCSMJumperInstalled())
            axisZPosition = galilCNStatusBean.getAxisCMotorPosition() / static_cast<real>(axisZStepPerMm);
        else
            axisZPosition = galilCNStatusBean.getAxisCReferencePosition() / static_cast<real>(axisZStepPerMm);

    }

public:

    bool getAxisXForwardLimit() const { return axisXForwardLimit; }
    void setAxisXForwardLimit(bool value) { axisXForwardLimit = value; }
    bool getAxisXReverseLimit() const { return axisXReverseLimit; }
    void setAxisXReverseLimit(bool value) { axisXReverseLimit = value; }
    bool getAxisXHomeInput() const { return axisXHomeInput; }
    void setAxisXHomeInput(bool value) { axisXHomeInput = value; }
    bool getAxisXMotorOff() const { return axisXMotorOff; }
    void setAxisXMotorOff(bool value) { axisXMotorOff = value; }
    real getAxisXPosition() const { return axisXPosition; }
    void setAxisXPosition(const real& value) { axisXPosition = value; }
    bool getAxisXMoveInProgress() const { return axisXMoveInProgress; }
    void setAxisXMoveInProgress(bool value) { axisXMoveInProgress = value; }


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

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::MotionBean)

#endif // MOTIONBEAN_HPP
