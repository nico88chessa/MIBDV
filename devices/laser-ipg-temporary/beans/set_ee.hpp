#ifndef IPG_LASER_TEMP_SET_EE_HPP
#define IPG_LASER_TEMP_SET_EE_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

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
