#ifndef GALILCNSTATUSBEAN_HPP
#define GALILCNSTATUSBEAN_HPP

#include <QMetaType>

#include <Constants.hpp>
#include <configure.h>
#include <devices/galil/GalilControllerUtils.hpp>

#include <gclib_record.h>

namespace PROGRAM_NAMESPACE {

class GalilCNStatusBean {
public:
    using Ptr = GalilCNStatusBean*;
    using ConstPtr = const GalilCNStatusBean*;

private:
    static constexpr unsigned int BIT_MASK_0 = 0x01 << 0;
    static constexpr unsigned int BIT_MASK_1 = 0x01 << 1;
    static constexpr unsigned int BIT_MASK_2 = 0x01 << 2;
    static constexpr unsigned int BIT_MASK_3 = 0x01 << 3;
    static constexpr unsigned int BIT_MASK_4 = 0x01 << 4;
    static constexpr unsigned int BIT_MASK_5 = 0x01 << 5;
    static constexpr unsigned int BIT_MASK_6 = 0x01 << 6;
    static constexpr unsigned int BIT_MASK_7 = 0x01 << 7;
    static constexpr unsigned int BIT_MASK_8 = 0x01 << 8;
    static constexpr unsigned int BIT_MASK_9 = 0x01 << 9;
    static constexpr unsigned int BIT_MASK_10 = 0x01 << 10;
    static constexpr unsigned int BIT_MASK_11 = 0x01 << 11;
    static constexpr unsigned int BIT_MASK_12 = 0x01 << 12;
    static constexpr unsigned int BIT_MASK_13 = 0x01 << 13;
    static constexpr unsigned int BIT_MASK_14 = 0x01 << 14;
    static constexpr unsigned int BIT_MASK_15 = 0x01 << 15;

private:
    // general status
    bool programRunning;
    bool waitingInputFromINCommand;
    bool traceOn;
    bool echoOn;
    unsigned char errorCode;
    bool needReset;
    bool isEmergencyMushroomPressed;

    // digital IO
    bool digitalInputs[GALIL_CN_DIGITAL_INPUTS];
    bool digitalOutputs[GALIL_CN_DIGITAL_OUTPUTS];

    /*** ASSE A ***/

    // axis switch
    bool axisALatchOccured;
    bool axisAStateLatchInput;
    bool axisAForwardLimit;
    bool axisAReverseLimit;
    bool axisAHomeInput;
    bool axisASMJumperInstalled;
    // axis status
    bool axisAMoveInProgress;
    bool axisAMotionPAPR;
    bool axisAMotionPA;
    bool axisAFeInProgress;
    bool axisAHmInProgress;
    bool axisAHm1Complete;
    bool axisAHm2Complete;
    bool axisACoordMotion;
    bool axisANegativeDirectionMove;
    bool axisAContourMotion;
    bool axisAMotionIsSlewing;
    bool axisAMotionStop2STorLimitSwitch;
    bool axisAMotionMakingFinaleEcal;
    bool axisALatchArmed;
    bool axisAOffOnErrorArmed;
    bool axisAMotorOff;
    // altro
    unsigned char axisAStopCode;
    int axisAReferencePosition;
    int axisAMotorPosition;
    int axisAPositionError;
    int axisAAuxPosition;
    int axisAVelocity;
    short axisATorque;
    unsigned short axisAAnalogIn;

    /*** ASSE B ***/

    // axis switch
    bool axisBLatchOccured;
    bool axisBStateLatchInput;
    bool axisBForwardLimit;
    bool axisBReverseLimit;
    bool axisBHomeInput;
    bool axisBSMJumperInstalled;
    // axis status
    bool axisBMoveInProgress;
    bool axisBMotionPAPR;
    bool axisBMotionPA;
    bool axisBFeInProgress;
    bool axisBHmInProgress;
    bool axisBHm1Complete;
    bool axisBHm2Complete;
    bool axisBCoordMotion;
    bool axisBNegativeDirectionMove;
    bool axisBContourMotion;
    bool axisBMotionIsSlewing;
    bool axisBMotionStop2STorLimitSwitch;
    bool axisBMotionMakingFinaleEcal;
    bool axisBLatchArmed;
    bool axisBOffOnErrorArmed;
    bool axisBMotorOff;
    // altro
    unsigned char axisBStopCode;
    int axisBReferencePosition;
    int axisBMotorPosition;
    int axisBPositionError;
    int axisBAuxPosition;
    int axisBVelocity;
    short axisBTorque;
    unsigned short axisBAnalogIn;

    /*** ASSE C ***/

    // axis switch
    bool axisCLatchOccured;
    bool axisCStateLatchInput;
    bool axisCForwardLimit;
    bool axisCReverseLimit;
    bool axisCHomeInput;
    bool axisCSMJumperInstalled;
    // axis status
    bool axisCMoveInProgress;
    bool axisCMotionPAPR;
    bool axisCMotionPA;
    bool axisCFeInProgress;
    bool axisCHmInProgress;
    bool axisCHm1Complete;
    bool axisCHm2Complete;
    bool axisCCoordMotion;
    bool axisCNegativeDirectionMove;
    bool axisCContourMotion;
    bool axisCMotionIsSlewing;
    bool axisCMotionStop2STorLimitSwitch;
    bool axisCMotionMakingFinaleEcal;
    bool axisCLatchArmed;
    bool axisCOffOnErrorArmed;
    bool axisCMotorOff;
    // altro
    unsigned short axisCStatus;
    unsigned char axisCSwitches;
    unsigned char axisCStopCode;
    int axisCReferencePosition;
    int axisCMotorPosition;
    int axisCPositionError;
    int axisCAuxPosition;
    int axisCVelocity;
    short axisCTorque;
    unsigned short axisCAnalogIn;

public:
    explicit GalilCNStatusBean() { }

    /**
     * NOTE NIC 31/10/2018 - questa e' una copy-initialization;
     * tuttavia nel compilatore gcc viene ottmizzata come una direct-initialization
     * metto explicit per forzare l'uso del tipo GalilCNStatusBean;
     * vedere \ref GalilCNController::getRecord
     */
    explicit GalilCNStatusBean(const GDataRecord2103& rec) {

        programRunning = rec.general_status & BIT_MASK_7;
        waitingInputFromINCommand = rec.general_status & BIT_MASK_2;
        traceOn = rec.general_status & BIT_MASK_1;
        echoOn = rec.general_status & BIT_MASK_0;
        errorCode = rec.error_code;

        digitalInputs[0] = rec.input_bank_0 & BIT_MASK_0;
        digitalInputs[1] = rec.input_bank_0 & BIT_MASK_1;
        digitalInputs[2] = rec.input_bank_0 & BIT_MASK_2;
        digitalInputs[3] = rec.input_bank_0 & BIT_MASK_3;
        digitalInputs[4] = rec.input_bank_0 & BIT_MASK_4;
        digitalInputs[5] = rec.input_bank_0 & BIT_MASK_5;
        digitalInputs[6] = rec.input_bank_0 & BIT_MASK_6;
        digitalInputs[7] = rec.input_bank_0 & BIT_MASK_7;

        digitalOutputs[0] = rec.output_bank_0 & BIT_MASK_0;
        digitalOutputs[1] = rec.output_bank_0 & BIT_MASK_1;
        digitalOutputs[2] = rec.output_bank_0 & BIT_MASK_2;
        digitalOutputs[3] = rec.output_bank_0 & BIT_MASK_3;
        digitalOutputs[4] = rec.output_bank_0 & BIT_MASK_4;
        digitalOutputs[5] = rec.output_bank_0 & BIT_MASK_5;
        digitalOutputs[6] = rec.output_bank_0 & BIT_MASK_6;
        digitalOutputs[7] = rec.output_bank_0 & BIT_MASK_7;

        axisALatchOccured = rec.axis_a_switches & BIT_MASK_7;
        axisAStateLatchInput = rec.axis_a_switches & BIT_MASK_6;
        axisAForwardLimit = rec.axis_a_switches & BIT_MASK_3;
        axisAReverseLimit = rec.axis_a_switches & BIT_MASK_2;
        axisAHomeInput = rec.axis_a_switches & BIT_MASK_1;
        axisASMJumperInstalled = rec.axis_a_switches & BIT_MASK_0;
        axisAMoveInProgress = rec.axis_a_status & BIT_MASK_15;
        axisAMotionPAPR = rec.axis_a_status & BIT_MASK_14;
        axisAMotionPA = rec.axis_a_status & BIT_MASK_13;
        axisAFeInProgress = rec.axis_a_status & BIT_MASK_12;
        axisAHmInProgress = rec.axis_a_status & BIT_MASK_11;
        axisAHm1Complete = rec.axis_a_status & BIT_MASK_10;
        axisAHm2Complete = rec.axis_a_status & BIT_MASK_9;
        axisACoordMotion = rec.axis_a_status & BIT_MASK_8;
        axisANegativeDirectionMove = rec.axis_a_status & BIT_MASK_7;
        axisAContourMotion = rec.axis_a_status & BIT_MASK_6;
        axisAMotionIsSlewing = rec.axis_a_status & BIT_MASK_5;
        axisAMotionStop2STorLimitSwitch = rec.axis_a_status & BIT_MASK_4;
        axisAMotionMakingFinaleEcal = rec.axis_a_status & BIT_MASK_3;
        axisALatchArmed = rec.axis_a_status & BIT_MASK_2;
        axisAOffOnErrorArmed = rec.axis_a_status & BIT_MASK_1;
        axisAMotorOff = rec.axis_a_status & BIT_MASK_0;
        axisAStopCode = rec.axis_a_stop_code;
        axisAReferencePosition = rec.axis_a_reference_position;
        axisAMotorPosition = rec.axis_a_motor_position;
        axisAPositionError = rec.axis_a_position_error;
        axisAAuxPosition = rec.axis_a_aux_position;
        axisAVelocity = rec.axis_a_velocity;
        axisATorque = rec.axis_a_torque;
        axisAAnalogIn = rec.axis_a_analog_in;

        axisBLatchOccured = rec.axis_b_switches & BIT_MASK_7;
        axisBStateLatchInput = rec.axis_b_switches & BIT_MASK_6;
        axisBForwardLimit = rec.axis_b_switches & BIT_MASK_3;
        axisBReverseLimit = rec.axis_b_switches & BIT_MASK_2;
        axisBHomeInput = rec.axis_b_switches & BIT_MASK_1;
        axisBSMJumperInstalled = rec.axis_b_switches & BIT_MASK_0;
        axisBMoveInProgress = rec.axis_b_status & BIT_MASK_15;
        axisBMotionPAPR = rec.axis_b_status & BIT_MASK_14;
        axisBMotionPA = rec.axis_b_status & BIT_MASK_13;
        axisBFeInProgress = rec.axis_b_status & BIT_MASK_12;
        axisBHmInProgress = rec.axis_b_status & BIT_MASK_11;
        axisBHm1Complete = rec.axis_b_status & BIT_MASK_10;
        axisBHm2Complete = rec.axis_b_status & BIT_MASK_9;
        axisBCoordMotion = rec.axis_b_status & BIT_MASK_8;
        axisBNegativeDirectionMove = rec.axis_b_status & BIT_MASK_7;
        axisBContourMotion = rec.axis_b_status & BIT_MASK_6;
        axisBMotionIsSlewing = rec.axis_b_status & BIT_MASK_5;
        axisBMotionStop2STorLimitSwitch = rec.axis_b_status & BIT_MASK_4;
        axisBMotionMakingFinaleEcal = rec.axis_b_status & BIT_MASK_3;
        axisBLatchArmed = rec.axis_b_status & BIT_MASK_2;
        axisBOffOnErrorArmed = rec.axis_b_status & BIT_MASK_1;
        axisBMotorOff = rec.axis_b_status & BIT_MASK_0;
        axisBStopCode = rec.axis_b_stop_code;
        axisBReferencePosition = rec.axis_b_reference_position;
        axisBMotorPosition = rec.axis_b_motor_position;
        axisBPositionError = rec.axis_b_position_error;
        axisBAuxPosition = rec.axis_b_aux_position;
        axisBVelocity = rec.axis_b_velocity;
        axisBTorque = rec.axis_b_torque;
        axisBAnalogIn = rec.axis_b_analog_in;

        axisCLatchOccured = rec.axis_c_switches & BIT_MASK_7;
        axisCStateLatchInput = rec.axis_c_switches & BIT_MASK_6;
        axisCForwardLimit = rec.axis_c_switches & BIT_MASK_3;
        axisCReverseLimit = rec.axis_c_switches & BIT_MASK_2;
        axisCHomeInput = rec.axis_c_switches & BIT_MASK_1;
        axisCSMJumperInstalled = rec.axis_c_switches & BIT_MASK_0;
        axisCMoveInProgress = rec.axis_c_status & BIT_MASK_15;
        axisCMotionPAPR = rec.axis_c_status & BIT_MASK_14;
        axisCMotionPA = rec.axis_c_status & BIT_MASK_13;
        axisCFeInProgress = rec.axis_c_status & BIT_MASK_12;
        axisCHmInProgress = rec.axis_c_status & BIT_MASK_11;
        axisCHm1Complete = rec.axis_c_status & BIT_MASK_10;
        axisCHm2Complete = rec.axis_c_status & BIT_MASK_9;
        axisCCoordMotion = rec.axis_c_status & BIT_MASK_8;
        axisCNegativeDirectionMove = rec.axis_c_status & BIT_MASK_7;
        axisCContourMotion = rec.axis_c_status & BIT_MASK_6;
        axisCMotionIsSlewing = rec.axis_c_status & BIT_MASK_5;
        axisCMotionStop2STorLimitSwitch = rec.axis_c_status & BIT_MASK_4;
        axisCMotionMakingFinaleEcal = rec.axis_c_status & BIT_MASK_3;
        axisCLatchArmed = rec.axis_c_status & BIT_MASK_2;
        axisCOffOnErrorArmed = rec.axis_c_status & BIT_MASK_1;
        axisCMotorOff = rec.axis_c_status & BIT_MASK_0;
        axisCStopCode = rec.axis_c_stop_code;
        axisCReferencePosition = rec.axis_c_reference_position;
        axisCMotorPosition = rec.axis_c_motor_position;
        axisCPositionError = rec.axis_c_position_error;
        axisCAuxPosition = rec.axis_c_aux_position;
        axisCVelocity = rec.axis_c_velocity;
        axisCTorque = rec.axis_c_torque;
        axisCAnalogIn = rec.axis_c_analog_in;

        needReset = false;
        isEmergencyMushroomPressed = false;
    }

    bool getProgramRunning() const { return programRunning; }
    void setProgramRunning(bool value) { programRunning = value; }
    bool getWaitingInputFromINCommand() const { return waitingInputFromINCommand; }
    void setWaitingInputFromINCommand(bool value) { waitingInputFromINCommand = value; }
    bool getTraceOn() const { return traceOn; }
    void setTraceOn(bool value) { traceOn = value; }
    bool getEchoOn() const { return echoOn; }
    void setEchoOn(bool value) { echoOn = value; }
    unsigned char getErrorCode() const { return errorCode; }
    void setErrorCode(unsigned char value) { errorCode = value; }
    bool getNeedReset() const { return needReset; }
    void setNeedReset(bool value) { needReset = value; }
    bool getIsEmergencyMushroomPressed() const { return isEmergencyMushroomPressed; }
    void setIsEmergencyMushroomPressed(bool value) { isEmergencyMushroomPressed = value; }

    bool getAxisALatchOccured() const { return axisALatchOccured; }
    void setAxisALatchOccured(bool value) { axisALatchOccured = value; }
    bool getAxisAStateLatchInput() const { return axisAStateLatchInput; }
    void setAxisAStateLatchInput(bool value) { axisAStateLatchInput = value; }
    bool getAxisAForwardLimit() const { return axisAForwardLimit; }
    void setAxisAForwardLimit(bool value) { axisAForwardLimit = value; }
    bool getAxisAReverseLimit() const { return axisAReverseLimit; }
    void setAxisAReverseLimit(bool value) { axisAReverseLimit = value; }
    bool getAxisAHomeInput() const { return axisAHomeInput; }
    void setAxisAHomeInput(bool value) { axisAHomeInput = value; }
    bool getAxisASMJumperInstalled() const { return axisASMJumperInstalled; }
    void setAxisASMJumperInstalled(bool value) { axisASMJumperInstalled = value; }
    bool getAxisAMoveInProgress() const { return axisAMoveInProgress; }
    void setAxisAMoveInProgress(bool value) { axisAMoveInProgress = value; }
    bool getAxisAMotionPAPR() const { return axisAMotionPAPR; }
    void setAxisAMotionPAPR(bool value) { axisAMotionPAPR = value; }
    bool getAxisAMotionPA() const { return axisAMotionPA; }
    void setAxisAMotionPA(bool value) { axisAMotionPA = value; }
    bool getAxisAFeInProgress() const { return axisAFeInProgress; }
    void setAxisAFeInProgress(bool value) { axisAFeInProgress = value; }
    bool getAxisAHmInProgress() const { return axisAHmInProgress; }
    void setAxisAHmInProgress(bool value) { axisAHmInProgress = value; }
    bool getAxisAHm1Complete() const { return axisAHm1Complete; }
    void setAxisAHm1Complete(bool value) { axisAHm1Complete = value; }
    bool getAxisAHm2Complete() const { return axisAHm2Complete; }
    void setAxisAHm2Complete(bool value) { axisAHm2Complete = value; }
    bool getAxisACoordMotion() const { return axisACoordMotion; }
    void setAxisACoordMotion(bool value) { axisACoordMotion = value; }
    bool getAxisANegativeDirectionMove() const { return axisANegativeDirectionMove; }
    void setAxisANegativeDirectionMove(bool value) { axisANegativeDirectionMove = value; }
    bool getAxisAContourMotion() const { return axisAContourMotion; }
    void setAxisAContourMotion(bool value) { axisAContourMotion = value; }
    bool getAxisAMotionIsSlewing() const { return axisAMotionIsSlewing; }
    void setAxisAMotionIsSlewing(bool value) { axisAMotionIsSlewing = value; }
    bool getAxisAMotionStop2STorLimitSwitch() const { return axisAMotionStop2STorLimitSwitch; }
    void setAxisAMotionStop2STorLimitSwitch(bool value) { axisAMotionStop2STorLimitSwitch = value; }
    bool getAxisAMotionMakingFinaleEcal() const { return axisAMotionMakingFinaleEcal; }
    void setAxisAMotionMakingFinaleEcal(bool value) { axisAMotionMakingFinaleEcal = value; }
    bool getAxisALatchArmed() const { return axisALatchArmed; }
    void setAxisALatchArmed(bool value) { axisALatchArmed = value; }
    bool getAxisAOffOnErrorArmed() const { return axisAOffOnErrorArmed; }
    void setAxisAOffOnErrorArmed(bool value) { axisAOffOnErrorArmed = value; }
    bool getAxisAMotorOff() const { return axisAMotorOff; }
    void setAxisAMotorOff(bool value) { axisAMotorOff = value; }
    unsigned char getAxisAStopCode() const { return axisAStopCode; }
    void setAxisAStopCode(unsigned char value) { axisAStopCode = value; }
    int getAxisAReferencePosition() const { return axisAReferencePosition; }
    void setAxisAReferencePosition(int value) { axisAReferencePosition = value; }
    int getAxisAMotorPosition() const { return axisAMotorPosition; }
    void setAxisAMotorPosition(int value) { axisAMotorPosition = value; }
    int getAxisAPositionError() const { return axisAPositionError; }
    void setAxisAPositionError(int value) { axisAPositionError = value; }
    int getAxisAAuxPosition() const { return axisAAuxPosition; }
    void setAxisAAuxPosition(int value) { axisAAuxPosition = value; }
    int getAxisAVelocity() const { return axisAVelocity; }
    void setAxisAVelocity(int value) { axisAVelocity = value; }
    void setAxisATorque(short value) { axisATorque = value; }
    short getAxisATorque() const { return axisATorque; }
    unsigned short getAxisAAnalogIn() const { return axisAAnalogIn; }
    void setAxisAAnalogIn(unsigned short value) { axisAAnalogIn = value; }

    bool getAxisBLatchOccured() const { return axisBLatchOccured; }
    void setAxisBLatchOccured(bool value) { axisBLatchOccured = value; }
    bool getAxisBStateLatchInput() const { return axisBStateLatchInput; }
    void setAxisBStateLatchInput(bool value) { axisBStateLatchInput = value; }
    bool getAxisBForwardLimit() const { return axisBForwardLimit; }
    void setAxisBForwardLimit(bool value) { axisBForwardLimit = value; }
    bool getAxisBReverseLimit() const { return axisBReverseLimit; }
    void setAxisBReverseLimit(bool value) { axisBReverseLimit = value; }
    bool getAxisBHomeInput() const { return axisBHomeInput; }
    void setAxisBHomeInput(bool value) { axisBHomeInput = value; }
    bool getAxisBSMJumperInstalled() const { return axisBSMJumperInstalled; }
    void setAxisBSMJumperInstalled(bool value) { axisBSMJumperInstalled = value; }
    bool getAxisBMoveInProgress() const { return axisBMoveInProgress; }
    void setAxisBMoveInProgress(bool value) { axisBMoveInProgress = value; }
    bool getAxisBMotionPAPR() const { return axisBMotionPAPR; }
    void setAxisBMotionPAPR(bool value) { axisBMotionPAPR = value; }
    bool getAxisBMotionPA() const { return axisBMotionPA; }
    void setAxisBMotionPA(bool value) { axisBMotionPA = value; }
    bool getAxisBFeInProgress() const { return axisBFeInProgress; }
    void setAxisBFeInProgress(bool value) { axisBFeInProgress = value; }
    bool getAxisBHmInProgress() const { return axisBHmInProgress; }
    void setAxisBHmInProgress(bool value) { axisBHmInProgress = value; }
    bool getAxisBHm1Complete() const { return axisBHm1Complete; }
    void setAxisBHm1Complete(bool value) { axisBHm1Complete = value; }
    bool getAxisBHm2Complete() const { return axisBHm2Complete; }
    void setAxisBHm2Complete(bool value) { axisBHm2Complete = value; }
    bool getAxisBCoordMotion() const { return axisBCoordMotion; }
    void setAxisBCoordMotion(bool value) { axisBCoordMotion = value; }
    bool getAxisBNegativeDirectionMove() const { return axisBNegativeDirectionMove; }
    void setAxisBNegativeDirectionMove(bool value) { axisBNegativeDirectionMove = value; }
    bool getAxisBContourMotion() const { return axisBContourMotion; }
    void setAxisBContourMotion(bool value) { axisBContourMotion = value; }
    bool getAxisBMotionIsSlewing() const { return axisBMotionIsSlewing; }
    void setAxisBMotionIsSlewing(bool value) { axisBMotionIsSlewing = value; }
    bool getAxisBMotionStop2STorLimitSwitch() const { return axisBMotionStop2STorLimitSwitch; }
    void setAxisBMotionStop2STorLimitSwitch(bool value) { axisBMotionStop2STorLimitSwitch = value; }
    bool getAxisBMotionMakingFinaleEcal() const { return axisBMotionMakingFinaleEcal; }
    void setAxisBMotionMakingFinaleEcal(bool value) { axisBMotionMakingFinaleEcal = value; }
    bool getAxisBLatchArmed() const { return axisBLatchArmed; }
    void setAxisBLatchArmed(bool value) { axisBLatchArmed = value; }
    bool getAxisBOffOnErrorArmed() const { return axisBOffOnErrorArmed; }
    void setAxisBOffOnErrorArmed(bool value) { axisBOffOnErrorArmed = value; }
    bool getAxisBMotorOff() const { return axisBMotorOff; }
    void setAxisBMotorOff(bool value) { axisBMotorOff = value; }
    unsigned char getAxisBStopCode() const { return axisBStopCode; }
    void setAxisBStopCode(unsigned char value) { axisBStopCode = value; }
    int getAxisBReferencePosition() const { return axisBReferencePosition; }
    void setAxisBReferencePosition(int value) { axisBReferencePosition = value; }
    int getAxisBMotorPosition() const { return axisBMotorPosition; }
    void setAxisBMotorPosition(int value) { axisBMotorPosition = value; }
    int getAxisBPositionError() const { return axisBPositionError; }
    void setAxisBPositionError(int value) { axisBPositionError = value; }
    int getAxisBAuxPosition() const { return axisBAuxPosition; }
    void setAxisBAuxPosition(int value) { axisBAuxPosition = value; }
    int getAxisBVelocity() const { return axisBVelocity; }
    void setAxisBVelocity(int value) { axisBVelocity = value; }
    short getAxisBTorque() const { return axisBTorque; }
    void setAxisBTorque(short value) { axisBTorque = value; }
    unsigned short getAxisBAnalogIn() const { return axisBAnalogIn; }
    void setAxisBAnalogIn(unsigned short value) { axisBAnalogIn = value; }

    bool getAxisCLatchOccured() const { return axisCLatchOccured; }
    void setAxisCLatchOccured(bool value) { axisCLatchOccured = value; }
    bool getAxisCStateLatchInput() const { return axisCStateLatchInput; }
    void setAxisCStateLatchInput(bool value) { axisCStateLatchInput = value; }
    bool getAxisCForwardLimit() const { return axisCForwardLimit; }
    void setAxisCForwardLimit(bool value) { axisCForwardLimit = value; }
    bool getAxisCReverseLimit() const { return axisCReverseLimit; }
    void setAxisCReverseLimit(bool value) { axisCReverseLimit = value; }
    bool getAxisCHomeInput() const { return axisCHomeInput; }
    void setAxisCHomeInput(bool value) { axisCHomeInput = value; }
    bool getAxisCSMJumperInstalled() const { return axisCSMJumperInstalled; }
    void setAxisCSMJumperInstalled(bool value) { axisCSMJumperInstalled = value; }
    bool getAxisCMoveInProgress() const { return axisCMoveInProgress; }
    void setAxisCMoveInProgress(bool value) { axisCMoveInProgress = value; }
    bool getAxisCMotionPAPR() const { return axisCMotionPAPR; }
    void setAxisCMotionPAPR(bool value) { axisCMotionPAPR = value; }
    bool getAxisCMotionPA() const { return axisCMotionPA; }
    void setAxisCMotionPA(bool value) { axisCMotionPA = value; }
    bool getAxisCFeInProgress() const { return axisCFeInProgress; }
    void setAxisCFeInProgress(bool value) { axisCFeInProgress = value; }
    bool getAxisCHmInProgress() const { return axisCHmInProgress; }
    void setAxisCHmInProgress(bool value) { axisCHmInProgress = value; }
    bool getAxisCHm1Complete() const { return axisCHm1Complete; }
    void setAxisCHm1Complete(bool value) { axisCHm1Complete = value; }
    bool getAxisCHm2Complete() const { return axisCHm2Complete; }
    void setAxisCHm2Complete(bool value) { axisCHm2Complete = value; }
    bool getAxisCCoordMotion() const { return axisCCoordMotion; }
    void setAxisCCoordMotion(bool value) { axisCCoordMotion = value; }
    bool getAxisCNegativeDirectionMove() const { return axisCNegativeDirectionMove; }
    void setAxisCNegativeDirectionMove(bool value) { axisCNegativeDirectionMove = value; }
    bool getAxisCContourMotion() const { return axisCContourMotion; }
    void setAxisCContourMotion(bool value) { axisCContourMotion = value; }
    bool getAxisCMotionIsSlewing() const { return axisCMotionIsSlewing; }
    void setAxisCMotionIsSlewing(bool value) { axisCMotionIsSlewing = value; }
    bool getAxisCMotionStop2STorLimitSwitch() const { return axisCMotionStop2STorLimitSwitch; }
    void setAxisCMotionStop2STorLimitSwitch(bool value) { axisCMotionStop2STorLimitSwitch = value; }
    bool getAxisCMotionMakingFinaleEcal() const { return axisCMotionMakingFinaleEcal; }
    void setAxisCMotionMakingFinaleEcal(bool value) { axisCMotionMakingFinaleEcal = value; }
    bool getAxisCLatchArmed() const { return axisCLatchArmed; }
    void setAxisCLatchArmed(bool value) { axisCLatchArmed = value; }
    bool getAxisCOffOnErrorArmed() const { return axisCOffOnErrorArmed; }
    void setAxisCOffOnErrorArmed(bool value) { axisCOffOnErrorArmed = value; }
    bool getAxisCMotorOff() const { return axisCMotorOff; }
    void setAxisCMotorOff(bool value) { axisCMotorOff = value; }
    unsigned short getAxisCStatus() const { return axisCStatus; }
    void setAxisCStatus(unsigned short value) { axisCStatus = value; }
    unsigned char getAxisCSwitches() const { return axisCSwitches; }
    void setAxisCSwitches(unsigned char value) { axisCSwitches = value; }
    unsigned char getAxisCStopCode() const { return axisCStopCode; }
    void setAxisCStopCode(unsigned char value) { axisCStopCode = value; }
    int getAxisCReferencePosition() const { return axisCReferencePosition; }
    void setAxisCReferencePosition(int value) { axisCReferencePosition = value; }
    int getAxisCMotorPosition() const { return axisCMotorPosition; }
    void setAxisCMotorPosition(int value) { axisCMotorPosition = value; }
    int getAxisCPositionError() const { return axisCPositionError; }
    void setAxisCPositionError(int value) { axisCPositionError = value; }
    int getAxisCAuxPosition() const { return axisCAuxPosition; }
    void setAxisCAuxPosition(int value) { axisCAuxPosition = value; }
    int getAxisCVelocity() const { return axisCVelocity; }
    void setAxisCVelocity(int value) { axisCVelocity = value; }
    short getAxisCTorque() const { return axisCTorque; }
    void setAxisCTorque(short value) { axisCTorque = value; }
    unsigned short getAxisCAnalogIn() const { return axisCAnalogIn; }
    void setAxisCAnalogIn(unsigned short value) { axisCAnalogIn = value; }

    // NOTE NIC 03/01/2019 - gli I/O del galil CN a differenza del PLC partono da 1 e non da 0
    bool getDigitalInput(int i) const {
        if (i<1 || i>=GALIL_CN_DIGITAL_INPUTS)
            throw DigitalInputOutOfBoundException();
        return digitalInputs[i-1];
    }
    bool getDigitalOutput(int i) const {
        if (i<1 || i>=GALIL_CN_DIGITAL_INPUTS)
            throw DigitalOutputOutOfBoundException();
        return digitalOutputs[i-1];
    }

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::GalilCNStatusBean)

#endif // GALILCNSTATUSBEAN_HPP
