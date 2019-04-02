#ifndef IPG_LASER_TEMP_SET_EM_HPP
#define IPG_LASER_TEMP_SET_EM_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class SetEMInput : public InputBean {
public:
    typedef SetEMInput* Ptr;
    typedef const SetEMInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 2;

private:
    IPG_USHORT modulationOn;

public:
    SetEMInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getModulationOn() const { return modulationOn; }

    void setModulationOn(const IPG_USHORT& value) { modulationOn = value; }
};

typedef OutputBean SetEMOutput;


}

Q_DECLARE_METATYPE(ipg::SetEMInput)

#endif // SET_EM_HPP
