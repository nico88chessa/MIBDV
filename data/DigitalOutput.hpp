#ifndef DIGITALOUTPUT_HPP
#define DIGITALOUTPUT_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

class DigitalOutput {
    using Ptr = DigitalOutput*;
    using ConstPtr = const DigitalOutput*;

private:
    QString name;
    int channel;
    bool invertLogic;
    DeviceType device;
    bool isAlarm;
    ElementType elementType;

public:
    DigitalOutput(QString name, int channel, bool invertLogic,
                  DeviceType device, bool isAlarm, ElementType elementType);

    QString getName() const;
    int getChannel() const;
    bool getInvertLogic() const;
    DeviceType getDevice() const;
    bool getIsAlarm() const;
    ElementType getElementType() const;

};

}

#endif // DIGITALOUTPUT_HPP
