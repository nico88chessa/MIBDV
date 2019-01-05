#ifndef ABSTRACTLASER_HPP
#define ABSTRACTLASER_HPP

#include <QString>

#include <configure.h>
#include <Types.hpp>
#include <devices/AbstractDevice.hpp>

namespace PROGRAM_NAMESPACE {

template <typename S, typename C, typename E>
class AbstractLaser : public AbstractDevice<S> {
public:
    using Ptr = AbstractLaser*;
    using ConstPtr = const AbstractLaser*;

public:
    virtual E reset() = 0;
    virtual E configure(const C& configuration) = 0;
    virtual E powerOn();
    virtual E powerOff();

    virtual bool isError(E errorCode) = 0;
    virtual QString decodeError(const E& errorCode) = 0;

};

template <typename S, typename C, typename E>
struct isDevice<AbstractLaser<S,C,E>> {
    static constexpr bool value = true;
    using statusType = S;
};

template <typename T>
struct isLaser {
    static constexpr bool value = false;
    using statusType = void*;
    using configType = void*;
    using errorType = void*;
};

template <typename S, typename C, typename E>
struct isLaser<AbstractLaser<S,C,E>> {
    static constexpr bool value = true;
    using statusType = S;
    using configType = C;
    using errorType = E;
};

}

#endif // ABSTRACTLASER_HPP
