#ifndef SET_PS_MODE_HPP
#define SET_PS_MODE_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

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
