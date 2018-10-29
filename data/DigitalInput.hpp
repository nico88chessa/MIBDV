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
};

}

#endif // DIGITALINPUT_HPP
