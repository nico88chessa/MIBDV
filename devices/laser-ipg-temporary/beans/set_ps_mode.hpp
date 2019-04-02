#ifndef IPG_LASER_TEMP_SET_PS_MODE_HPP
#define IPG_LASER_TEMP_SET_PS_MODE_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class SetPSModeInput : public InputBean {
public:
    typedef SetPSModeInput* Ptr;
    typedef const SetPSModeInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 2;

private:
    IPG_USHORT powerSupply;

public:
    SetPSModeInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getPowerSupply() const { return powerSupply; }

    void setPowerSupply(const IPG_USHORT& value) { powerSupply = value; }

};

typedef OutputBean SetPSModeOutput;

}

Q_DECLARE_METATYPE(ipg::SetPSModeInput)

#endif // SET_PS_MODE_HPP
