#ifndef SET_EE_HPP
#define SET_EE_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

class SetEEInput : public InputBean {
public:
    typedef SetEEInput* Ptr;
    typedef const SetEEInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 2;

private:
    IPG_USHORT emissionOn;

public:
    SetEEInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getEmissionOn() const { return emissionOn; }

    void setEmissionOn(const IPG_USHORT& value) { emissionOn = value; }

};

typedef OutputBean SetEEOutput;


}

Q_DECLARE_METATYPE(ipg::SetEEInput)

#endif // SET_EE_HPP
