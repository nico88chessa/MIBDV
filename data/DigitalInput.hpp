#ifndef DIGITALINPUT_HPP
#define DIGITALINPUT_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>


namespace PROGRAM_NAMESPACE {

class DigitalInput {
public:
    using Ptr = DigitalInput*;
    using ConstPtr = const DigitalInput*;

private:
    QString name;
    int channel;
    bool invertLogic;
    DeviceKey device;
    bool isAlarm;
    bool isAlarmInverted;
    IOType elementType;
    MachineStatus alarmOnMachineStatus;

public:
    DigitalInput() :
        DigitalInput("", DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, false, IOType::NOT_VALID, MachineStatus::STATUS_NAN) { }

    DigitalInput(QString name, int channel, bool invertLogic,
                 DeviceKey device, bool isAlarm, bool isAlarmInverted, IOType elementType,
                 MachineStatus alarmOnMachineStatus) :
        name(name), channel(channel), invertLogic(invertLogic),
        device(device), isAlarm(isAlarm), isAlarmInverted(isAlarmInverted), elementType(elementType),
        alarmOnMachineStatus(alarmOnMachineStatus) { }

    inline QString getName() const { return name; }
    inline int getChannel() const { return channel; }
    inline bool getInvertLogic() const { return invertLogic; }
    inline DeviceKey getDevice() const { return device; }
    inline bool getIsAlarm() const { return isAlarm; }
    inline bool getIsAlarmInverted() const { return isAlarmInverted; }
    inline IOType getElementType() const { return elementType; }
    MachineStatus getAlarmOnMachineStatus() const { return alarmOnMachineStatus; }

    friend inline bool operator==(const DigitalInput& l, const DigitalInput& r);
    friend inline bool operator<(const DigitalInput& l, const DigitalInput& r);

};

bool operator==(const DigitalInput& l, const DigitalInput& r) {
    return (l.device == r.device) && (l.channel == r.channel);
}

bool operator<(const DigitalInput& l, const DigitalInput& r) {
    return (l.device < r.device) || (
                (l.device == r.device) && (l.channel < r.channel));
}

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DigitalInput)

#endif // DIGITALINPUT_HPP
