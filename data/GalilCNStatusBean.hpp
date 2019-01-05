#ifndef GALILCNSTATUSBEAN_HPP
#define GALILCNSTATUSBEAN_HPP

#include <QMetaType>

#include <Constants.hpp>
#include <configure.h>
#include <devices/motion/galil/GalilControllerUtils.hpp>

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
     * NOTE NIC 31/10/2018: questa e' una copy-initialization;
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
    }

    bool getProgramRunning() const { return programRunning; }
    bool getWaitingInputFromINCommand() const { return waitingInputFromINCommand; }
    bool getTraceOn() const { return traceOn; }
    bool getEchoOn() const { return echoOn; }
    unsigned char getErrorCode() const { return errorCode; }
    bool getAxisALatchOccured() const { return axisALatchOccured; }
    bool getAxisAStateLatchInput() const { return axisAStateLatchInput; }
    bool getAxisAForwardLimit() const { return axisAForwardLimit; }
    bool getAxisAReverseLimit() const { return axisAReverseLimit; }
    bool getAxisAHomeInput() const { return axisAHomeInput; }
    bool getAxisASMJumperInstalled() const { return axisASMJumperInstalled; }
    bool getAxisAMoveInProgress() const { return axisAMoveInProgress; }
    bool getAxisAMotionPAPR() const { return axisAMotionPAPR; }
    bool getAxisAMotionPA() const { return axisAMotionPA; }
    bool getAxisAFeInProgress() const { return axisAFeInProgress; }
    bool getAxisAHmInProgress() const { return axisAHmInProgress; }
    bool getAxisAHm1Complete() const { return axisAHm1Complete; }
    bool getAxisAHm2Complete() const { return axisAHm2Complete; }
    bool getAxisACoordMotion() const { return axisACoordMotion; }
    bool getAxisANegativeDirectionMove() const { return axisANegativeDirectionMove; }
    bool getAxisAContourMotion() const { return axisAContourMotion; }
    bool getAxisAMotionIsSlewing() const { return axisAMotionIsSlewing; }
    bool getAxisAMotionStop2STorLimitSwitch() const { return axisAMotionStop2STorLimitSwitch; }
    bool getAxisAMotionMakingFinaleEcal() const { return axisAMotionMakingFinaleEcal; }
    bool getAxisALatchArmed() const { return axisALatchArmed; }
    bool getAxisAOffOnErrorArmed() const { return axisAOffOnErrorArmed; }
    bool getAxisAMotorOff() const { return axisAMotorOff; }
    unsigned char getAxisAStopCode() const { return axisAStopCode; }
    int getAxisAReferencePosition() const { return axisAReferencePosition; }
    int getAxisAMotorPosition() const { return axisAMotorPosition; }
    int getAxisAPositionError() const { return axisAPositionError; }
    int getAxisAAuxPosition() const { return axisAAuxPosition; }
    int getAxisAVelocity() const { return axisAVelocity; }
    short getAxisATorque() const { return axisATorque; }
    unsigned short getAxisAAnalogIn() const { return axisAAnalogIn; }
    bool getAxisBLatchOccured() const { return axisBLatchOccured; }
    bool getAxisBStateLatchInput() const { return axisBStateLatchInput; }
    bool getAxisBForwardLimit() const { return axisBForwardLimit; }
    bool getAxisBReverseLimit() const { return axisBReverseLimit; }
    bool getAxisBHomeInput() const { return axisBHomeInput; }
    bool getAxisBSMJumperInstalled() const { return axisBSMJumperInstalled; }
    bool getAxisBMoveInProgress() const { return axisBMoveInProgress; }
    bool getAxisBMotionPAPR() const { return axisBMotionPAPR; }
    bool getAxisBMotionPA() const { return axisBMotionPA; }
    bool getAxisBFeInProgress() const { return axisBFeInProgress; }
    bool getAxisBHmInProgress() const { return axisBHmInProgress; }
    bool getAxisBHm1Complete() const { return axisBHm1Complete; }
    bool getAxisBHm2Complete() const { return axisBHm2Complete; }
    bool getAxisBCoordMotion() const { return axisBCoordMotion; }
    bool getAxisBNegativeDirectionMove() const { return axisBNegativeDirectionMove; }
    bool getAxisBContourMotion() const { return axisBContourMotion; }
    bool getAxisBMotionIsSlewing() const { return axisBMotionIsSlewing; }
    bool getAxisBMotionStop2STorLimitSwitch() const { return axisBMotionStop2STorLimitSwitch; }
    bool getAxisBMotionMakingFinaleEcal() const { return axisBMotionMakingFinaleEcal; }
    bool getAxisBLatchArmed() const { return axisBLatchArmed; }
    bool getAxisBOffOnErrorArmed() const { return axisBOffOnErrorArmed; }
    bool getAxisBMotorOff() const { return axisBMotorOff; }
    unsigned char getAxisBStopCode() const { return axisBStopCode; }
    int getAxisBReferencePosition() const { return axisBReferencePosition; }
    int getAxisBMotorPosition() const { return axisBMotorPosition; }
    int getAxisBPositionError() const { return axisBPositionError; }
    int getAxisBAuxPosition() const { return axisBAuxPosition; }
    int getAxisBVelocity() const { return axisBVelocity; }
    short getAxisBTorque() const { return axisBTorque; }
    unsigned short getAxisBAnalogIn() const { return axisBAnalogIn; }
    bool getAxisCLatchOccured() const { return axisCLatchOccured; }
    bool getAxisCStateLatchInput() const { return axisCStateLatchInput; }
    bool getAxisCForwardLimit() const { return axisCForwardLimit; }
    bool getAxisCReverseLimit() const { return axisCReverseLimit; }
    bool getAxisCHomeInput() const { return axisCHomeInput; }
    bool getAxisCSMJumperInstalled() const { return axisCSMJumperInstalled; }
    bool getAxisCMoveInProgress() const { return axisCMoveInProgress; }
    bool getAxisCMotionPAPR() const { return axisCMotionPAPR; }
    bool getAxisCMotionPA() const { return axisCMotionPA; }
    bool getAxisCFeInProgress() const { return axisCFeInProgress; }
    bool getAxisCHmInProgress() const { return axisCHmInProgress; }
    bool getAxisCHm1Complete() const { return axisCHm1Complete; }
    bool getAxisCHm2Complete() const { return axisCHm2Complete; }
    bool getAxisCCoordMotion() const { return axisCCoordMotion; }
    bool getAxisCNegativeDirectionMove() const { return axisCNegativeDirectionMove; }
    bool getAxisCContourMotion() const { return axisCContourMotion; }
    bool getAxisCMotionIsSlewing() const { return axisCMotionIsSlewing; }
    bool getAxisCMotionStop2STorLimitSwitch() const { return axisCMotionStop2STorLimitSwitch; }
    bool getAxisCMotionMakingFinaleEcal() const { return axisCMotionMakingFinaleEcal; }
    bool getAxisCLatchArmed() const { return axisCLatchArmed; }
    bool getAxisCOffOnErrorArmed() const { return axisCOffOnErrorArmed; }
    bool getAxisCMotorOff() const { return axisCMotorOff; }
    unsigned short getAxisCStatus() const { return axisCStatus; }
    unsigned char getAxisCSwitches() const { return axisCSwitches; }
    unsigned char getAxisCStopCode() const { return axisCStopCode; }
    int getAxisCReferencePosition() const { return axisCReferencePosition; }
    int getAxisCMotorPosition() const { return axisCMotorPosition; }
    int getAxisCPositionError() const { return axisCPositionError; }
    int getAxisCAuxPosition() const { return axisCAuxPosition; }
    int getAxisCVelocity() const { return axisCVelocity; }
    short getAxisCTorque() const { return axisCTorque; }
    unsigned short getAxisCAnalogIn() const { return axisCAnalogIn; }

    // NOTE NIC 03/01/2019: gli I/O del galil CN a differenza del PLC partono da 1 e non da 0
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
