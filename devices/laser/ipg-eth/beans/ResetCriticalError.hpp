#ifndef RESET_CRITICAL_ERROR_HPP
#define RESET_CRITICAL_ERROR_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

class ResetCriticalErrorInput : public InputBean {
public:
    typedef ResetCriticalErrorInput* Ptr;
    typedef const ResetCriticalErrorInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 10;

private:
    QString data;

public:
    ResetCriticalErrorInput() : InputBean(INPUT_DATA_SIZE) { }

    QString getData() const { return data; }

    void setData(const QString& value) { data = value; }

};

typedef OutputBean ResetCriticalErrorOutput;


}

Q_DECLARE_METATYPE(ipg::ResetCriticalErrorInput)


#endif // RESET_CRITICAL_ERROR_HPP
