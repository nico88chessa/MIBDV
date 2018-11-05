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
    IOType elementType;

public:
    DigitalInput();
    DigitalInput(QString name, int channel, bool invertLogic,
                 DeviceKey device, bool isAlarm, IOType elementType);

    QString getName() const;
    int getChannel() const;
    bool getInvertLogic() const;
    DeviceKey getDevice() const;
    bool getIsAlarm() const;
    IOType getElementType() const;

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

#endif // DIGITALINPUT_HPP
