#ifndef GALILCONTROLLERUTILS_HPP
#define GALILCONTROLLERUTILS_HPP

#include <exception>

#include <gclibo.h>
#include <gclib_errors.h>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>

#include <QString>

namespace PROGRAM_NAMESPACE {

class DigitalInputOutOfBoundException : public std::exception {
    virtual const char* what() const noexcept {
        return "Digital input out of bound exception";
    }
};

class DigitalOutputOutOfBoundException : public std::exception {
    virtual const char* what() const noexcept {
        return "Digital output out of bound exception";
    }
};

class AnalogInputOutOfBoundException : public std::exception {
    virtual const char* what() const noexcept {
        return "Digital output out of bound exception";
    }
};

class AnalogOutputOutOfBoundException : public std::exception {
    virtual const char* what() const noexcept {
        return "Digital output out of bound exception";
    }
};

static constexpr int GALIL_CN_DIGITAL_INPUTS = 8;
static constexpr int GALIL_CN_DIGITAL_OUTPUTS = 8;

static constexpr int GALIL_CN_STOP_CODE_MOTORS_RUNNING_INDEPENDENT_MODE = 0;
static constexpr int GALIL_CN_STOP_CODE_MOTORS_DECELERATING_STOP_COMMANDED_INDEPENDENT_POSITION = 1;
static constexpr int GALIL_CN_STOP_CODE_DECELERATING_STOPPED_FWD_LIMIT_SWITCH_OR_SOFT_LIMIT_FL = 2;
static constexpr int GALIL_CN_STOP_CODE_DECELERATING_STOPPED_REV_LIMIT_SWITCH_OR_SOFT_LIMIT_BL = 3;
static constexpr int GALIL_CN_STOP_CODE_DECELERATING_STOP_COMMAND = 4;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_ABORT_INPUT = 6;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_ABORT_COMMAND = 7;
static constexpr int GALIL_CN_STOP_CODE_DECELERATING_STOPPED_OFF_ON_ERROR = 8;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_AFTER_FINDING_EDGE = 9;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX = 10;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_SELECTIVE_ABORT_INPUT = 11;
static constexpr int GALIL_CN_STOP_CODE_DECELERATING_STOPPED_ENCODER_FAILURE = 12;
static constexpr int GALIL_CN_STOP_CODE_AMPLIFIER_FAULT = 15;
static constexpr int GALIL_CN_STOP_CODE_STEPPER_POSITION_MAINTENANCE_ERROR = 16;
static constexpr int GALIL_CN_STOP_CODE_RUNNING_IN_PVT_MODE = 30;
static constexpr int GALIL_CN_STOP_CODE_PVT_MODE_COMPLETED_NORMALLY = 31;
static constexpr int GALIL_CN_STOP_CODE_PVT_MODE_EXITED_BECAUSE_BUFFER_IS_EMPTY = 32;
static constexpr int GALIL_CN_STOP_CODE_CONTOUR_RUNNING = 50;
static constexpr int GALIL_CN_STOP_CODE_CONTOUR_STOPPED = 51;
static constexpr int GALIL_CN_STOP_CODE_ECAM_RUNNING = 60;
static constexpr int GALIL_CN_STOP_CODE_ECAM_STOPPED = 61;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_ETHERCAT_COMMUNICATION_FAILURE = 70;
static constexpr int GALIL_CN_STOP_CODE_STOPPED_ETHERCAT_DRIVE_FAULT = 71;
static constexpr int GALIL_CN_STOP_CODE_MC_TIMEOUT = 99;
static constexpr int GALIL_CN_STOP_CODE_VECTOR_SEQUENCE_RUNNING = 100;
static constexpr int GALIL_CN_STOP_CODE_VECTOR_SEQUENCE_STOPPED = 101;

static constexpr int GALIL_PLC_DIGITAL_INPUTS = 16;
static constexpr int GALIL_PLC_DIGITAL_OUTPUTS = 16;
static constexpr int GALIL_PLC_ANALOG_INPUTS = 8;
static constexpr int GALIL_PLC_ANALOG_OUTPUTS = 8;
static constexpr unsigned int GALIL_PLC_ANALOG_COUNT_MAX_VALUE = 32572;

static constexpr int G_CUSTOM_CN_NON_INIZIALIZZATO = PROGRAM_ERR_START_CODE + 1;
static constexpr int G_CUSTOM_CN_DIGITAL_INPUT_OUT_OF_RANGE = PROGRAM_ERR_START_CODE + 2;
static constexpr int G_CUSTOM_CN_DIGITAL_OUTPUT_OUT_OF_RANGE = PROGRAM_ERR_START_CODE + 3;
static constexpr int G_CUSTOM_CN_ANALOG_INPUT_OUT_OF_RANGE = PROGRAM_ERR_START_CODE + 4;
static constexpr int G_CUSTOM_CN_NOT_CONNECTED = PROGRAM_ERR_START_CODE + 5;

static constexpr int G_CUSTOM_PLC_NON_INIZIALIZZATO = PROGRAM_ERR_START_CODE + 6;
static constexpr int G_CUSTOM_PLC_DIGITAL_INPUT_OUT_OF_RANGE = PROGRAM_ERR_START_CODE + 7;
static constexpr int G_CUSTOM_PLC_DIGITAL_OUTPUT_OUT_OF_RANGE = PROGRAM_ERR_START_CODE + 8;
static constexpr int G_CUSTOM_PLC_ANALOG_INPUT_OUT_OF_RANGE = PROGRAM_ERR_START_CODE + 9;
static constexpr int G_CUSTOM_PLC_NOT_CONNECTED = PROGRAM_ERR_START_CODE + 10;

class GalilControllerUtils {

public:
    static QString decodeError(int error);
    static QString getErrorDescription(int error);
    static QString getTCDescription(int rcCode);
    static QString getCustomErrorDescription(int errorCode);
    static QString getStopCodeDescription(int stopCode);
    static MotionStopCode evaluateStopCode(int stopCode);

};

}

#endif // GALILCONTROLLERUTILS_HPP
