#ifndef GALILPLCSTATUSBEAN_HPP
#define GALILPLCSTATUSBEAN_HPP

#include <QMetaType>

#include <Constants.hpp>
#include <configure.h>

#include <devices/galil/GalilControllerUtils.hpp>

#include <gclib_record.h>

namespace PROGRAM_NAMESPACE {

class GalilPLCStatusBean {
public:
    using Ptr = GalilPLCStatusBean*;
    using ConstPtr = const GalilPLCStatusBean*;

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
    bool programRunning;
    bool waitingInputFromINCommand;
    bool traceOn;
    bool echoOn;
    unsigned char errorCode;

    bool digitalInputs[GALIL_PLC_DIGITAL_INPUTS];
    bool digitalOutputs[GALIL_PLC_DIGITAL_INPUTS];
    analogReal analogInputs[GALIL_PLC_ANALOG_INPUTS];
    analogReal analogOutputs[GALIL_PLC_ANALOG_OUTPUTS];

public:
    GalilPLCStatusBean() { }

    /**
     * NOTE NIC 31/10/2018: questa e' una copy-initialization;
     * tuttavia nel compilatore gcc viene ottmizzata come una direct-initialization
     * metto explicit per forzare l'uso del tipo GalilPLCStatusBean;
     * vedere \ref GalilPLCController::getRecord
     */
    explicit GalilPLCStatusBean(const GDataRecord47000_ENC& rec) {

        programRunning = rec.general_status & BIT_MASK_7;
        waitingInputFromINCommand = rec.general_status & BIT_MASK_2;
        traceOn = rec.general_status & BIT_MASK_1;
        echoOn = rec.general_status & BIT_MASK_0;
        errorCode = rec.error_code ;

        analogOutputs[0] = rec.output_analog_0;
        analogOutputs[1] = rec.output_analog_1;
        analogOutputs[2] = rec.output_analog_2;
        analogOutputs[3] = rec.output_analog_3;
        analogOutputs[4] = rec.output_analog_4;
        analogOutputs[5] = rec.output_analog_5;
        analogOutputs[6] = rec.output_analog_6;
        analogOutputs[7] = rec.output_analog_7;

        analogInputs[0] = rec.input_analog_0;
        analogInputs[1] = rec.input_analog_1;
        analogInputs[2] = rec.input_analog_2;
        analogInputs[3] = rec.input_analog_3;
        analogInputs[4] = rec.input_analog_4;
        analogInputs[5] = rec.input_analog_5;
        analogInputs[6] = rec.input_analog_6;
        analogInputs[7] = rec.input_analog_7;

        digitalInputs[0] = rec.input_bank_0 & BIT_MASK_0;
        digitalInputs[1] = rec.input_bank_0 & BIT_MASK_1;
        digitalInputs[2] = rec.input_bank_0 & BIT_MASK_2;
        digitalInputs[3] = rec.input_bank_0 & BIT_MASK_3;
        digitalInputs[4] = rec.input_bank_0 & BIT_MASK_4;
        digitalInputs[5] = rec.input_bank_0 & BIT_MASK_5;
        digitalInputs[6] = rec.input_bank_0 & BIT_MASK_6;
        digitalInputs[7] = rec.input_bank_0 & BIT_MASK_7;
        digitalInputs[8] = rec.input_bank_0 & BIT_MASK_8;
        digitalInputs[9] = rec.input_bank_0 & BIT_MASK_9;
        digitalInputs[10] = rec.input_bank_0 & BIT_MASK_10;
        digitalInputs[11] = rec.input_bank_0 & BIT_MASK_11;
        digitalInputs[12] = rec.input_bank_0 & BIT_MASK_12;
        digitalInputs[13] = rec.input_bank_0 & BIT_MASK_13;
        digitalInputs[14] = rec.input_bank_0 & BIT_MASK_14;
        digitalInputs[15] = rec.input_bank_0 & BIT_MASK_15;

        digitalOutputs[0] = rec.output_bank_0 & BIT_MASK_0;
        digitalOutputs[1] = rec.output_bank_0 & BIT_MASK_1;
        digitalOutputs[2] = rec.output_bank_0 & BIT_MASK_2;
        digitalOutputs[3] = rec.output_bank_0 & BIT_MASK_3;
        digitalOutputs[4] = rec.output_bank_0 & BIT_MASK_4;
        digitalOutputs[5] = rec.output_bank_0 & BIT_MASK_5;
        digitalOutputs[6] = rec.output_bank_0 & BIT_MASK_6;
        digitalOutputs[7] = rec.output_bank_0 & BIT_MASK_7;
        digitalOutputs[8] = rec.output_bank_0 & BIT_MASK_8;
        digitalOutputs[9] = rec.output_bank_0 & BIT_MASK_9;
        digitalOutputs[10] = rec.output_bank_0 & BIT_MASK_10;
        digitalOutputs[11] = rec.output_bank_0 & BIT_MASK_11;
        digitalOutputs[12] = rec.output_bank_0 & BIT_MASK_12;
        digitalOutputs[13] = rec.output_bank_0 & BIT_MASK_13;
        digitalOutputs[14] = rec.output_bank_0 & BIT_MASK_14;
        digitalOutputs[15] = rec.output_bank_0 & BIT_MASK_15;
    }

    bool getTraceOn() const { return traceOn; }
    bool getEchoOn() const { return echoOn; }
    unsigned char getErrorCode() const { return errorCode; }
    bool getProgramRunning() const { return programRunning; }
    bool getWaitingInputFromINCommand() const { return waitingInputFromINCommand; }
    bool getDigitalInput(int i) const {
        // TODO NIC 31/10/2018 - gestire eccezione di indice fuori range (usare eccezione)
        return digitalInputs[i];
    }
    bool getDigitalOutput(int i) const {
        // TODO NIC 31/10/2018 - gestire eccezione di indice fuori range (usare eccezione)
        return digitalOutputs[i];
    }
    analogReal getAnalogInput(int i) const {
        // TODO NIC 31/10/2018 - gestire eccezione di indice fuori range (usare eccezione)
        return analogInputs[i];
    }
    analogReal getAnalogOutput(int i) const {
        // TODO NIC 31/10/2018 - gestire eccezione di indice fuori range (usare eccezione)
        return analogOutputs[i];
    }

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::GalilPLCStatusBean)

#endif // GALILPLCSTATUSBEAN_HPP
