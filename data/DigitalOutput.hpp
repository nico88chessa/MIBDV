#ifndef DIGITALOUTPUT_HPP
#define DIGITALOUTPUT_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

class DigitalOutput {
public:
    using Ptr = DigitalOutput*;
    using ConstPtr = const DigitalOutput*;

private:
    QString name;
    int channel;
    bool invertLogic;
    DeviceKey device;
    bool isAlarm;
    IOType elementType;

public:
    DigitalOutput() :
        DigitalOutput("", -1, false, DeviceKey::NONE, false, IOType::NOT_VALID) { }

    DigitalOutput(QString name, int channel, bool invertLogic,
                  DeviceKey device, bool isAlarm, IOType elementType) :
              name(name), channel(channel), invertLogic(invertLogic),
              device(device), isAlarm(isAlarm), elementType(elementType) { }

    inline QString getName() const { return name; }
    inline int getChannel() const { return channel; }
    inline bool getInvertLogic() const { return invertLogic; }
    inline DeviceKey getDevice() const { return device; }
    inline bool getIsAlarm() const { return isAlarm; }
    inline IOType getElementType() const { return elementType; }

    friend inline bool operator==(const DigitalOutput& l, const DigitalOutput& r);
    friend inline bool operator<(const DigitalOutput& l, const DigitalOutput& r);

};

bool operator==(const DigitalOutput& l, const DigitalOutput& r) {
    return (l.device == r.device) && (l.channel == r.channel);
}

bool operator<(const DigitalOutput& l, const DigitalOutput& r) {
    return (l.device < r.device) || (
                (l.device == r.device) && (l.channel < r.channel));
}

}

#endif // DIGITALOUTPUT_HPP
