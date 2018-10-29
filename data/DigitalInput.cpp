#include "DigitalInput.hpp"

using namespace PROGRAM_NAMESPACE;

DigitalInput::DigitalInput(
        QString name, int channel, bool invertLogic,
        DeviceKey device, bool isAlarm, IOType elementType) :
    name(name), channel(channel), invertLogic(invertLogic),
    device(device), isAlarm(isAlarm), elementType(elementType) { }

QString DigitalInput::getName() const { return name; }

int DigitalInput::getChannel() const { return channel; }

bool DigitalInput::getInvertLogic() const { return invertLogic; }

DeviceKey DigitalInput::getDevice() const { return device; }

bool DigitalInput::getIsAlarm() const { return isAlarm; }

IOType DigitalInput::getElementType() const { return elementType; }

