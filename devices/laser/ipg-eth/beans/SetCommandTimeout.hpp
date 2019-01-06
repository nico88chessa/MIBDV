#ifndef SET_COMMAND_TIMEOUT_HPP
#define SET_COMMAND_TIMEOUT_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

class SetCommandTimeoutInput : public InputBean {
public:
    typedef SetCommandTimeoutInput* Ptr;
    typedef const SetCommandTimeoutInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 4;

private:
    IPG_ULONG timeout;

public:
    SetCommandTimeoutInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_ULONG getTimeout() const { return timeout; }

    void setTimeout(const IPG_ULONG& value) { timeout = value; }

};

typedef OutputBean SetCommandTimeoutOutput;


}

Q_DECLARE_METATYPE(ipg::SetCommandTimeoutInput)

#endif // SET_COMMAND_TIMEOUT_HPP
