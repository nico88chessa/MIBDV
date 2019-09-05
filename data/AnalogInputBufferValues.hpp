#ifndef ANALOGINPUTBUFFERVALUES_HPP
#define ANALOGINPUTBUFFERVALUES_HPP

#include "AnalogInput.hpp"
#include "NumericBuffer.hpp"

namespace PROGRAM_NAMESPACE {

class AnalogInputBufferValues : public AnalogInput {
public:
    using Ptr = AnalogInputBufferValues*;
    using ConstPtr = AnalogInputBufferValues*;

private:
    using AnalogBuffer = NumericBuffer<analogReal>;
    AnalogBuffer values;

public:
    AnalogInputBufferValues(int bufferSize = AnalogBuffer::DEFAULT_SAMPLE_ARRAY_SIZE) :
        AnalogInputBufferValues(AnalogInput(), bufferSize) { }

    AnalogInputBufferValues(const AnalogInput& aIn, int bufferSize = AnalogBuffer::DEFAULT_SAMPLE_ARRAY_SIZE) :
        AnalogInput(
            aIn.getName(),
            aIn.getChannel(),
            aIn.getDevice(),
            aIn.getIsAlarm(),
            aIn.getGain(),
            aIn.getOffset(),
            aIn.getUnit(),
            aIn.getLowerLimit(),
            aIn.getUpperLimit(),
            aIn.getHysteresys(),
            aIn.getElementType()
            ), values(bufferSize) { }

    AnalogInputBufferValues(QString name, int channel, DeviceKey device,
                     bool isAlarm, analogReal gain, analogReal offset,
                     QString unit, analogReal lowerLimit,
                     analogReal upperLimit, analogReal hysteresys,
                     IOType elementType = IOType::GENERIC_ANALOG_INPUT,
                     int bufferSize = AnalogBuffer::DEFAULT_SAMPLE_ARRAY_SIZE) :
        AnalogInput(
            name, channel, device,
            isAlarm, gain, offset,
            unit, lowerLimit,
            upperLimit, hysteresys,
            elementType), values(bufferSize) { }

    analogReal getAverage() const { return values.getAverage(); }
    void addValue(const analogReal& value) { values.addValue(value); }

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::AnalogInputBufferValues)

#endif // ANALOGINPUTBUFFERVALUES_HPP
