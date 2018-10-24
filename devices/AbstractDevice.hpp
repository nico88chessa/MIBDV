#ifndef ABSTRACTDEVICE_HPP
#define ABSTRACTDEVICE_HPP

#include <configure.h>

namespace PROGRAM_NAMESPACE {

template <typename S>
class AbstractDevice {

    using Ptr = AbstractDevice*;
    using ConstPtr = const AbstractDevice*;

public:
    virtual bool isConnected() const = 0;
    virtual S getStatus() const = 0;

};

// type traits

template <typename T>
struct isDevice {
    static constexpr bool value = false;
    using statusType = T;
};

template <typename T>
struct isDevice<AbstractDevice<T> > {
    static constexpr bool value = true;
    using statusType = T;
};

}


#endif // ABSTRACTDEVICE_HPP
