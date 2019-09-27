#include "AnalogInput.hpp"

using namespace PROGRAM_NAMESPACE;

AnalogInput::AnalogInput() :
    AnalogInput("", ANALOG_INPUT_CHANNEL_NONE, DeviceKey::NONE,
                  false, 1.0, 0,
                  "", std::numeric_limits<analogReal>::min(),
                  std::numeric_limits<analogReal>::max(), 1.0, MachineStatus::STATUS_NAN) { }

AnalogInput::AnalogInput(
        QString name, int channel, DeviceKey device,
        bool isAlarm, analogReal gain, analogReal offset,
        QString unit, analogReal lowerLimit,
        analogReal upperLimit, analogReal hysteresys, MachineStatus alarmOnMachineStatus,
        IOType elementType) :
        name(name), channel(channel), device(device),
        isAlarm(isAlarm), gain(gain), offset(offset),
        unit(unit), lowerLimit(lowerLimit),
        upperLimit(upperLimit), hysteresys(hysteresys), alarmOnMachineStatus(alarmOnMachineStatus),
        elementType(elementType) { }

QString AnalogInput::getName() const { return name; }

int AnalogInput::getChannel() const { return channel; }

DeviceKey AnalogInput::getDevice() const { return device; }

bool AnalogInput::getIsAlarm() const { return isAlarm; }

analogReal AnalogInput::getGain() const { return gain; }

analogReal AnalogInput::getOffset() const { return offset; }

QString AnalogInput::getUnit() const { return unit; }

analogReal AnalogInput::getLowerLimit() const { return lowerLimit; }

analogReal AnalogInput::getUpperLimit() const { return upperLimit; }

analogReal AnalogInput::getHysteresys() const { return hysteresys; }

IOType AnalogInput::getElementType() const { return elementType; }

MachineStatus AnalogInput::getAlarmOnMachineStatus() const { return alarmOnMachineStatus; }
