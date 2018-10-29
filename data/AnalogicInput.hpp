#ifndef ANALOGICINPUT_HPP
#define ANALOGICINPUT_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

class AnalogicInput {
public:
    using Ptr = AnalogicInput*;
    using ConstPtr = const AnalogicInput*;

private:
    QString name;
    int channel;
    DeviceKey device;
    bool isAlarm;
    analogicReal gain;
    analogicReal offset;
    QString unit;
    analogicReal lowerLimit;
    analogicReal upperLimit;
    analogicReal hysteresys;
    IOType elementType;

public:
    AnalogicInput(
            QString name, int channel, DeviceKey device,
            bool isAlarm, IOType elementType, analogicReal gain,
            analogicReal offset, QString unit, analogicReal lowerLimit,
            analogicReal upperLimit, analogicReal hysteresys);

    QString getName() const;
    int getChannel() const;
    DeviceKey getDevice() const;
    bool getIsAlarm() const;
    IOType getElementType() const;
    analogicReal getGain() const;
    analogicReal getOffset() const;
    QString getUnit() const;
    analogicReal getLowerLimit() const;
    analogicReal getUpperLimit() const;
    analogicReal getHysteresys() const;
};

}

#endif // ANALOGICINPUT_HPP
