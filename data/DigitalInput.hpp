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
    DeviceType device;
    bool isAlarm;
    ElementType elementType;

public:
    DigitalInput(QString name, int channel, bool invertLogic,
                 DeviceType device, bool isAlarm, ElementType elementType);

    QString getName() const;
    int getChannel() const;
    bool getInvertLogic() const;
    DeviceType getDevice() const;
    bool getIsAlarm() const;
    ElementType getElementType() const;
};

}

#endif // DIGITALINPUT_HPP
