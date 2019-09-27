#ifndef ANALOGINPUTVALUE_HPP
#define ANALOGINPUTVALUE_HPP

#include "AnalogInput.hpp"

namespace PROGRAM_NAMESPACE {

class AnalogInputValue : public AnalogInput {
public:
    using Ptr = AnalogInputValue*;
    using ConstPtr = AnalogInputValue*;

private:
    analogReal value;

public:
    AnalogInputValue() :
        AnalogInputValue(AnalogInput()) { }

    AnalogInputValue(const AnalogInput& aIn) :
        AnalogInput(
            aIn.getName(),
            aIn.getChannel(),
            aIn.getDevice(),
            aIn.getIsAlarm(),
            aIn.getGain(),
            aIn.getOffset(),
            aIn.getUnit(),
            aIn.getLowerLimit(),
            aIn.getUpperLimit(),
            aIn.getHysteresys(),
            aIn.getAlarmOnMachineStatus(),
            aIn.getElementType()) { }

    AnalogInputValue(QString name, int channel, DeviceKey device,
                     bool isAlarm, analogReal gain, analogReal offset,
                     QString unit, analogReal lowerLimit,
                     analogReal upperLimit, analogReal hysteresys,
                     MachineStatus alarmOnMachineStatus,
                     IOType elementType = IOType::GENERIC_ANALOG_INPUT) :
        AnalogInput(
            name, channel, device,
            isAlarm, gain, offset,
            unit, lowerLimit,
            upperLimit, hysteresys, alarmOnMachineStatus,
            elementType), value(0) { }

    analogReal getValue() const { return value; }
    void setValue(const analogReal& value) { this->value = value; }

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::AnalogInputValue)

#endif // ANALOGINPUTVALUE_HPP
