#ifndef ABSTRACTDEVICE_HPP
#define ABSTRACTDEVICE_HPP

#include <exception>
#include <type_traits>

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

};


template <typename S>
class AbstractDevice : public IAbstractDevice {
public:
    using Ptr = AbstractDevice*;
    using ConstPtr = const AbstractDevice*;

    using Status = S;

public:
    virtual S getStatus() = 0;

};


// type traits

template <typename T, typename = void>
struct isDevice {
    static constexpr bool value = false;
};

template <typename T>
struct isDevice<
        T,
        std::enable_if_t<std::is_base_of<AbstractDevice<typename T::Status>, T>::value>
        > {
    static constexpr bool value = true;
    using statusType = typename T::Status;
};

}


#endif // ABSTRACTDEVICE_HPP
