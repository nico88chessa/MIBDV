#include "AnalogicInput.hpp"

using namespace PROGRAM_NAMESPACE;

AnalogicInput::AnalogicInput(
        QString name, int channel, DeviceKey device,
        bool isAlarm, /*IOType elementType, */analogicReal gain,
        analogicReal offset, QString unit, analogicReal lowerLimit,
        analogicReal upperLimit, analogicReal hysteresys) :
        name(name), channel(channel), device(device),
        isAlarm(isAlarm), /*elementType(elementType), */gain(gain),
        offset(offset), unit(unit), lowerLimit(lowerLimit),
        upperLimit(upperLimit), hysteresys(hysteresys) { }

QString AnalogicInput::getName() const { return name; }

int AnalogicInput::getChannel() const { return channel; }

DeviceKey AnalogicInput::getDevice() const { return device; }

bool AnalogicInput::getIsAlarm() const { return isAlarm; }

//IOType AnalogicInput::getElementType() const { return elementType; }

analogicReal AnalogicInput::getGain() const { return gain; }

analogicReal AnalogicInput::getOffset() const { return offset; }

QString AnalogicInput::getUnit() const { return unit; }

analogicReal AnalogicInput::getLowerLimit() const { return lowerLimit; }

analogicReal AnalogicInput::getUpperLimit() const { return upperLimit; }

analogicReal AnalogicInput::getHysteresys() const { return hysteresys; }
