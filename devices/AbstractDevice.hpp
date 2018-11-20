#ifndef ABSTRACTDEVICE_HPP
#define ABSTRACTDEVICE_HPP

#include <configure.h>

namespace PROGRAM_NAMESPACE {

class IAbstractDevice {
public:
    using Ptr = IAbstractDevice*;
    using ConstPtr = const IAbstractDevice*;

public:
    virtual bool isConnected() const = 0;
    virtual ~IAbstractDevice() { }

};

template <typename S>
class AbstractDevice : public IAbstractDevice {
public:
    using Ptr = AbstractDevice*;
    using ConstPtr = const AbstractDevice*;

public:
    virtual S getStatus() = 0;

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
