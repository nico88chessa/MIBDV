#ifndef ABTRACTPLC_HPP
#define ABTRACTPLC_HPP

#include <QString>

#include <configure.h>
#include <Types.hpp>
#include "AbstractDevice.hpp"

namespace PROGRAM_NAMESPACE {

template <typename S, typename E>
class AbstractPLC : public AbstractDevice<S> {
public:
    using Ptr = AbstractPLC*;
    using ConstPtr = AbstractPLC*;

public:
    virtual E getDigitalInput(int input, int& inputStatus) = 0;
    virtual E getDigitalOutput(int output, int& outputStatus) = 0;
    virtual E getAnalogInput(int analogInput, anlType& analogInputStatus) = 0;
    virtual E setDigitalOutput(int output, bool value) = 0;
    virtual bool isError(E errorCode) = 0;
    virtual QString decodeError(const E& errorCode) = 0;

};

// type traits

template <typename T>
struct isPLC {
    static constexpr bool value = false;
    using statusType = void*;
    using errorType = void*;
};

template <typename S, typename E>
struct isPLC<AbstractPLC<S, E>> {
    static constexpr bool value = true;
    using statusType = S;
    using errorType = E;
};

template <typename S, typename E>
struct isDevice<AbstractPLC<S, E> > {
    static constexpr bool value = true;
    using statusType = S;
};


}

#endif // ABTRACTPLC_HPP
