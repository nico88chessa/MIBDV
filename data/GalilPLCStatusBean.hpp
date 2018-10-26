#ifndef GALILPLCSTATUSBEAN_HPP
#define GALILPLCSTATUSBEAN_HPP

#include <Constants.hpp>
#include <configure.h>

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

    unsigned short outputAnalog1;
    unsigned short outputAnalog2;
    unsigned short outputAnalog3;
    unsigned short outputAnalog4;
    unsigned short outputAnalog5;
    unsigned short outputAnalog6;
    unsigned short outputAnalog7;
    unsigned short outputAnalog8;

    unsigned short inputAnalog1;
    unsigned short inputAnalog2;
    unsigned short inputAnalog3;
    unsigned short inputAnalog4;
    unsigned short inputAnalog5;
    unsigned short inputAnalog6;
    unsigned short inputAnalog7;
    unsigned short inputAnalog8;

    bool digitalInput1;
    bool digitalInput2;
    bool digitalInput3;
    bool digitalInput4;
    bool digitalInput5;
    bool digitalInput6;
    bool digitalInput7;
    bool digitalInput8;
    bool digitalInput9;
    bool digitalInput10;
    bool digitalInput11;
    bool digitalInput12;
    bool digitalInput13;
    bool digitalInput14;
    bool digitalInput15;
    bool digitalInput16;

    bool digitalOutput1;
    bool digitalOutput2;
    bool digitalOutput3;
    bool digitalOutput4;
    bool digitalOutput5;
    bool digitalOutput6;
    bool digitalOutput7;
    bool digitalOutput8;
    bool digitalOutput9;
    bool digitalOutput10;
    bool digitalOutput11;
    bool digitalOutput12;
    bool digitalOutput13;
    bool digitalOutput14;
    bool digitalOutput15;
    bool digitalOutput16;

public:
    GalilPLCStatusBean(const GDataRecord47000_ENC& rec) {

        programRunning = rec.general_status & BIT_MASK_7;
        waitingInputFromINCommand = rec.general_status & BIT_MASK_2;
        traceOn = rec.general_status & BIT_MASK_1;
        echoOn = rec.general_status & BIT_MASK_0;
        errorCode = rec.error_code ;

        outputAnalog1 = rec.output_analog_0;
        outputAnalog2 = rec.output_analog_1;
        outputAnalog3 = rec.output_analog_2;
        outputAnalog4 = rec.output_analog_3;
        outputAnalog5 = rec.output_analog_4;
        outputAnalog6 = rec.output_analog_5;
        outputAnalog7 = rec.output_analog_6;
        outputAnalog8 = rec.output_analog_7;

        inputAnalog1 = rec.input_analog_0;
        inputAnalog2 = rec.input_analog_1;
        inputAnalog3 = rec.input_analog_2;
        inputAnalog4 = rec.input_analog_3;
        inputAnalog5 = rec.input_analog_4;
        inputAnalog6 = rec.input_analog_5;
        inputAnalog7 = rec.input_analog_6;
        inputAnalog8 = rec.input_analog_7;

        digitalInput1 = rec.input_bank_0 & BIT_MASK_0;
        digitalInput2 = rec.input_bank_0 & BIT_MASK_1;
        digitalInput3 = rec.input_bank_0 & BIT_MASK_2;
        digitalInput4 = rec.input_bank_0 & BIT_MASK_3;
        digitalInput5 = rec.input_bank_0 & BIT_MASK_4;
        digitalInput6 = rec.input_bank_0 & BIT_MASK_5;
        digitalInput7 = rec.input_bank_0 & BIT_MASK_6;
        digitalInput8 = rec.input_bank_0 & BIT_MASK_7;
        digitalInput9 = rec.input_bank_0 & BIT_MASK_8;
        digitalInput10 = rec.input_bank_0 & BIT_MASK_9;
        digitalInput11 = rec.input_bank_0 & BIT_MASK_10;
        digitalInput12 = rec.input_bank_0 & BIT_MASK_11;
        digitalInput13 = rec.input_bank_0 & BIT_MASK_12;
        digitalInput14 = rec.input_bank_0 & BIT_MASK_13;
        digitalInput15 = rec.input_bank_0 & BIT_MASK_14;
        digitalInput16 = rec.input_bank_0 & BIT_MASK_15;

        digitalOutput1 = rec.output_bank_0 & BIT_MASK_0;
        digitalOutput2 = rec.output_bank_0 & BIT_MASK_1;
        digitalOutput3 = rec.output_bank_0 & BIT_MASK_2;
        digitalOutput4 = rec.output_bank_0 & BIT_MASK_3;
        digitalOutput5 = rec.output_bank_0 & BIT_MASK_4;
        digitalOutput6 = rec.output_bank_0 & BIT_MASK_5;
        digitalOutput7 = rec.output_bank_0 & BIT_MASK_6;
        digitalOutput8 = rec.output_bank_0 & BIT_MASK_7;
        digitalOutput9 = rec.output_bank_0 & BIT_MASK_8;
        digitalOutput10 = rec.output_bank_0 & BIT_MASK_9;
        digitalOutput11 = rec.output_bank_0 & BIT_MASK_10;
        digitalOutput12 = rec.output_bank_0 & BIT_MASK_11;
        digitalOutput13 = rec.output_bank_0 & BIT_MASK_12;
        digitalOutput14 = rec.output_bank_0 & BIT_MASK_13;
        digitalOutput15 = rec.output_bank_0 & BIT_MASK_14;
        digitalOutput16 = rec.output_bank_0 & BIT_MASK_15;
    }

    bool getTraceOn() const { return traceOn; }
    bool getEchoOn() const { return echoOn; }
    unsigned char getErrorCode() const { return errorCode; }
    unsigned short getOutputAnalog1() const { return outputAnalog1; }
    unsigned short getOutputAnalog2() const { return outputAnalog2; }
    unsigned short getOutputAnalog3() const { return outputAnalog3; }
    unsigned short getOutputAnalog4() const { return outputAnalog4; }
    unsigned short getOutputAnalog5() const { return outputAnalog5; }
    unsigned short getOutputAnalog6() const { return outputAnalog6; }
    unsigned short getOutputAnalog7() const { return outputAnalog7; }
    unsigned short getOutputAnalog8() const { return outputAnalog8; }
    unsigned short getInputAnalog1() const { return inputAnalog1; }
    unsigned short getInputAnalog2() const { return inputAnalog2; }
    unsigned short getInputAnalog3() const { return inputAnalog3; }
    unsigned short getInputAnalog4() const { return inputAnalog4; }
    unsigned short getInputAnalog5() const { return inputAnalog5; }
    unsigned short getInputAnalog6() const { return inputAnalog6; }
    unsigned short getInputAnalog7() const { return inputAnalog7; }
    unsigned short getInputAnalog8() const { return inputAnalog8; }
    bool getDigitalInput1() const { return digitalInput1; }
    bool getDigitalInput2() const { return digitalInput2; }
    bool getDigitalInput3() const { return digitalInput3; }
    bool getDigitalInput4() const { return digitalInput4; }
    bool getDigitalInput5() const { return digitalInput5; }
    bool getDigitalInput6() const { return digitalInput6; }
    bool getDigitalInput7() const { return digitalInput7; }
    bool getDigitalInput8() const { return digitalInput8; }
    bool getDigitalInput9() const { return digitalInput9; }
    bool getDigitalInput10() const { return digitalInput10; }
    bool getDigitalInput11() const { return digitalInput11; }
    bool getDigitalInput12() const { return digitalInput12; }
    bool getDigitalInput13() const { return digitalInput13; }
    bool getDigitalInput14() const { return digitalInput14; }
    bool getDigitalInput15() const { return digitalInput15; }
    bool getDigitalInput16() const { return digitalInput16; }
    bool getDigitalOutput1() const { return digitalOutput1; }
    bool getDigitalOutput2() const { return digitalOutput2; }
    bool getDigitalOutput3() const { return digitalOutput3; }
    bool getDigitalOutput4() const { return digitalOutput4; }
    bool getDigitalOutput5() const { return digitalOutput5; }
    bool getDigitalOutput6() const { return digitalOutput6; }
    bool getDigitalOutput7() const { return digitalOutput7; }
    bool getDigitalOutput8() const { return digitalOutput8; }
    bool getDigitalOutput9() const { return digitalOutput9; }
    bool getDigitalOutput10() const { return digitalOutput10; }
    bool getDigitalOutput11() const { return digitalOutput11; }
    bool getDigitalOutput12() const { return digitalOutput12; }
    bool getDigitalOutput13() const { return digitalOutput13; }
    bool getDigitalOutput14() const { return digitalOutput14; }
    bool getDigitalOutput15() const { return digitalOutput15; }
    bool getDigitalOutput16() const { return digitalOutput16; }
    bool getProgramRunning() const { return programRunning; }
    bool getWaitingInputFromINCommand() const { return waitingInputFromINCommand; }

};


}

#endif // GALILPLCSTATUSBEAN_HPP
