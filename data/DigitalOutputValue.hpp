#ifndef DIGITALOUTPUTVALUE_HPP
#define DIGITALOUTPUTVALUE_HPP

#include "DigitalOutput.hpp"

namespace PROGRAM_NAMESPACE {

class DigitalOutputValue : public DigitalOutput {
public:
    using Ptr = DigitalOutputValue*;
    using ConstPtr = const DigitalOutputValue*;

private:
    bool value;

public:
    DigitalOutputValue() :
        DigitalOutputValue(DigitalOutput()) { }

    DigitalOutputValue(const DigitalOutput& dOut) :
        DigitalOutputValue(
            dOut.getName(),
            dOut.getChannel(),
            dOut.getInvertLogic(),
            dOut.getDevice(),
            dOut.getIsAlarm(),
            dOut.getElementType()) { }

    DigitalOutputValue(QString name, int channel, bool invertLogic,
                 DeviceKey device, bool isAlarm, IOType elementType) :
        DigitalOutput(name, channel, invertLogic, device, isAlarm, elementType), value(false) { }

    bool getValue() const { return value; }
    void setValue(bool value) { this->value = value; }

};

}

#endif // DIGITALOUTPUTVALUE_HPP
