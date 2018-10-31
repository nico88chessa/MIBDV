#ifndef ANALOGINPUT_HPP
#define ANALOGINPUT_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

class AnalogInput {
public:
    using Ptr = AnalogInput*;
    using ConstPtr = const AnalogInput*;

private:
    QString name;
    int channel;
    DeviceKey device;
    bool isAlarm;
    analogReal gain;
    analogReal offset;
    QString unit;
    analogReal lowerLimit;
    analogReal upperLimit;
    analogReal hysteresys;
    IOType elementType;

public:
    AnalogInput();
    AnalogInput(
            QString name, int channel, DeviceKey device,
            bool isAlarm, analogReal gain, analogReal offset,
            QString unit, analogReal lowerLimit,
            analogReal upperLimit, analogReal hysteresys,
            IOType elementType = IOType::GENERIC_ANALOG_INPUT);

    QString getName() const;
    int getChannel() const;
    DeviceKey getDevice() const;
    bool getIsAlarm() const;
    analogReal getGain() const;
    analogReal getOffset() const;
    QString getUnit() const;
    analogReal getLowerLimit() const;
    analogReal getUpperLimit() const;
    analogReal getHysteresys() const;
    IOType getElementType() const;
};

}

#endif // ANALOGINPUT_HPP
