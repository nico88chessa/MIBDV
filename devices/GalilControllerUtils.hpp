#ifndef GALILCONTROLLERUTILS_HPP
#define GALILCONTROLLERUTILS_HPP

#include <gclibo.h>
#include <gclib_errors.h>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>

#include <QString>

namespace PROGRAM_NAMESPACE {

static constexpr int GALIL_CN_DIGITAL_INPUTS = 8;
static constexpr int GALIL_CN_DIGITAL_OUTPUTS = 8;

static constexpr int GALIL_PLC_DIGITAL_INPUTS = 16;
static constexpr int GALIL_PLC_DIGITAL_OUTPUTS = 16;
static constexpr int GALIL_PLC_ANALOG_INPUTS = 8;
static constexpr int GALIL_PLC_ANALOG_OUTPUTS = 8;

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
};

}

#endif // GALILCONTROLLERUTILS_HPP
