#ifndef IPG_LASER_TEMP_SET_MODE_INDEX_HPP
#define IPG_LASER_TEMP_SET_MODE_INDEX_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class SetModeIndexInput : public InputBean {
public:
    typedef SetModeIndexInput* Ptr;
    typedef const SetModeIndexInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 2;

private:
    IPG_USHORT modeIndex;

public:
    SetModeIndexInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getModeIndex() const { return modeIndex; }

    void setModeIndex(const IPG_USHORT& value) { modeIndex = value; }

};

typedef OutputBean SetModeIndexOutput;


}

Q_DECLARE_METATYPE(ipg::SetModeIndexInput)

#endif // SET_MODE_INDEX_HPP
