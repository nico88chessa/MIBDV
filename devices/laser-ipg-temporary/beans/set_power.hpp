#ifndef IPG_LASER_TEMP_SET_POWER_HPP
#define IPG_LASER_TEMP_SET_POWER_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class SetPowerInput : public InputBean {
public:
    typedef SetPowerInput* Ptr;
    typedef const SetPowerInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 4;

private:
    IPG_FLOAT power;

public:
    SetPowerInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_FLOAT getPower() const { return power; }

    void setPower(const IPG_FLOAT& value) { power = value; }

};


class SetPowerOutput : public OutputBean {
public:
    typedef SetPowerOutput* Ptr;
    typedef const SetPowerOutput* ConstPtr;

private:
    IPG_USHORT result;

public:
    IPG_USHORT getResult() const { return result; }

    void setResult(const IPG_USHORT& value) { result = value; }

};


}

Q_DECLARE_METATYPE(ipg::SetPowerInput)
Q_DECLARE_METATYPE(ipg::SetPowerOutput)

#endif // SET_POWER_HPP
