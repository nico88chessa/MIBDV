#include "DigitalOutput.hpp"

using namespace PROGRAM_NAMESPACE;

DigitalOutput::DigitalOutput(
        QString name, int channel, bool invertLogic,
        DeviceKey device, bool isAlarm, IOType elementType) :
    name(name), channel(channel), invertLogic(invertLogic),
    device(device), isAlarm(isAlarm), elementType(elementType) { }

QString DigitalOutput::getName() const { return name; }

int DigitalOutput::getChannel() const { return channel; }

bool DigitalOutput::getInvertLogic() const { return invertLogic; }

DeviceKey DigitalOutput::getDevice() const { return device; }

bool DigitalOutput::getIsAlarm() const { return isAlarm; }

IOType DigitalOutput::getElementType() const { return elementType; }

