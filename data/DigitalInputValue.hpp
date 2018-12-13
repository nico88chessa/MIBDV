#ifndef DIGITALINPUTVALUE_HPP
#define DIGITALINPUTVALUE_HPP

#include "DigitalInput.hpp"

namespace PROGRAM_NAMESPACE {

class DigitalInputValue : public DigitalInput {
public:
    using Ptr = DigitalInputValue*;
    using ConstPtr = const DigitalInputValue*;

private:
    bool value;

public:
    DigitalInputValue() :
        DigitalInputValue(DigitalInput()) { }

    DigitalInputValue(const DigitalInput& dOut) :
        DigitalInputValue(
            dOut.getName(),
            dOut.getChannel(),
            dOut.getInvertLogic(),
            dOut.getDevice(),
            dOut.getIsAlarm(),
            dOut.getElementType()) { }

    DigitalInputValue(QString name, int channel, bool invertLogic,
                 DeviceKey device, bool isAlarm, IOType elementType) :
        DigitalInput(name, channel, invertLogic, device, isAlarm, elementType), value(false) { }

    bool getValue() const { return value; }
    void setValue(bool value) { this->value = value; }

};

}

#endif // DIGITALINPUTVALUE_HPP
