#ifndef GET_CRITICAL_ERROR_COUNTER_HPP
#define GET_CRITICAL_ERROR_COUNTER_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

typedef InputBean GetCriticalErrorCounterInput;

class GetCriticalErrorCounterOutput : public OutputBean {
public:
    typedef GetCriticalErrorCounterOutput* Ptr;
    typedef const GetCriticalErrorCounterOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_ULONG criticalErrorsCounter;

public:

    IPG_USHORT getVersion() const { return version; }

    IPG_ULONG getCriticalErrorsCounter() const { return criticalErrorsCounter; }

    void setCriticalErrorsCounter(const IPG_ULONG& value) { criticalErrorsCounter = value; }

    void setVersion(const IPG_USHORT& value) { version = value; }
};


}

Q_DECLARE_METATYPE(ipg::GetCriticalErrorCounterOutput)

#endif // GET_CRITICAL_ERROR_COUNTER_HPP
