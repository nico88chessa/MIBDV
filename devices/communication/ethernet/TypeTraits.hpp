#ifndef COMMUNICATION_ETHERNET_TYPE_TRAITS_HPP
#define COMMUNICATION_ETHERNET_TYPE_TRAITS_HPP

#include <utility>

#include "MarshallerInterface.hpp"
#include "AbstractMarshaller.hpp"

namespace communication {

namespace ethernet {

template <typename M>
struct isMarshallerInterface {
    static constexpr bool value = std::is_base_of<MarshallerInterface, M>::value;
};

//template <typename M, typename = void>
//struct isMarshaller {
//    static constexpr bool value = false;
//};

template <typename M, typename = void>
struct isMarshaller {
    static constexpr bool value = false;
};

template <typename M>
struct isMarshaller<M, std::enable_if_t<isMarshallerInterface<M>::value>> {
    static constexpr bool value = true;
};

template <template <typename, typename> class M, typename I, typename O>
struct isMarshaller<class M<I,O>, std::enable_if_t<isMarshallerInterface<M<I,O>>::value>> {
    static constexpr bool value = true;
};

template <typename M, typename = void>
struct isAbstractMarshaller {
    static constexpr bool value = false;
    using inputType = void*;
    using outputType = void*;
};

template <template <typename, typename> class M, typename I, typename O>
struct isAbstractMarshaller<
        M<I,O>,
        std::enable_if_t<std::is_base_of<AbstractMarshaller<I,O>, M<I,O>>::value>> {
    static constexpr bool value = true;
    using inputType = I;
    using outputType = O;
};

}

}

#endif // SOCKET_SYNC_EXECUTOR_HPP
