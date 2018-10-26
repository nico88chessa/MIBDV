#ifndef ABTRACTPLC_HPP
#define ABTRACTPLC_HPP

#include <configure.h>
#include "AbstractDevice.hpp"

namespace PROGRAM_NAMESPACE {

template <typename S, typename E>
class AbstractPLC : public AbstractDevice<S> {
public:
    using Ptr = AbstractPLC*;
    using ConstPtr = AbstractPLC*;

public:
    using anlType = double;

public:
    E getDigitalInput(int input, int& inputStatus);
    E getDigitalOutput(int output, int& outputStatus);
    E getAnalogInput(int analogInput, anlType& analogInputStatus);
    E setDigitalOutput(int output, bool value);

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
