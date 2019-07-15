#ifndef ABSTRACTDEVICE_HPP
#define ABSTRACTDEVICE_HPP

#include <exception>

#include <configure.h>

namespace PROGRAM_NAMESPACE {

enum class DeviceKey;

class NoStatusException : public std::exception {
    const char* what() const noexcept {
        return "No status exception: status is not available from device";
    }
};

class IAbstractDevice {
public:
    using Ptr = IAbstractDevice*;
    using ConstPtr = const IAbstractDevice*;

public:
    virtual bool isConnected() = 0;
    virtual bool connect() = 0;
    virtual ~IAbstractDevice() { }

    /* TODO NIC 15/07/2019 - valutare se usare deviceKey come parametro di template
     * nella classe AbstractDevice; quindi, da
     * AbstractDevice<S> portarla a AbstractDevice<S, DeviceKey> e utilizzare
     * i traits dove necessario
     */
    virtual DeviceKey getDeviceKey() const = 0;

};

template <typename S>
class AbstractDevice : public IAbstractDevice {
public:
    using Ptr = AbstractDevice*;
    using ConstPtr = const AbstractDevice*;

    using status = S;

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
