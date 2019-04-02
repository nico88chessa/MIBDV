#ifndef IPG_LASER_TEMP_SET_FREQUENCY_HPP
#define IPG_LASER_TEMP_SET_FREQUENCY_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class SetFrequencyInput : public InputBean {
public:
    typedef SetFrequencyInput* Ptr;
    typedef const SetFrequencyInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 4;

private:
    IPG_FLOAT frequency;

public:
    SetFrequencyInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_FLOAT getFrequency() const { return frequency; }

    void setFrequency(const IPG_FLOAT& value) { frequency = value; }

};


class SetFrequencyOutput : public OutputBean {
public:
    typedef SetFrequencyOutput* Ptr;
    typedef const SetFrequencyOutput* ConstPtr;

private:
    IPG_USHORT result;

public:
    IPG_USHORT getResult() const { return result; }

    void setResult(const IPG_USHORT& value) { result = value; }

};


}

Q_DECLARE_METATYPE(ipg::SetFrequencyInput)
Q_DECLARE_METATYPE(ipg::SetFrequencyOutput)

#endif // SET_FREQUENCY_HPP
