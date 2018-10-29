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
    DigitalOutput(QString name, int channel, bool invertLogic,
                  DeviceKey device, bool isAlarm, IOType elementType);

    QString getName() const;
    int getChannel() const;
    bool getInvertLogic() const;
    DeviceKey getDevice() const;
    bool getIsAlarm() const;
    IOType getElementType() const;

};

}

#endif // DIGITALOUTPUT_HPP
