#ifndef IPG_TYPE_TRAITS_HPP
#define IPG_TYPE_TRAITS_HPP

#include <utility>

#include <communication/ethernet/TypeTraits.hpp>

namespace ipg {

template <typename I, typename O> class IpgMarshaller;

namespace ce = communication::ethernet;

template <typename T>
struct isIpgMarshaller {
    static constexpr bool value = false;
};

template <typename I, typename O>
struct isIpgMarshaller<IpgMarshaller<I,O>> {
    static constexpr bool value = true;
    using inputType = I;
    using outputType = O;
};

}

namespace communication {

namespace ethernet {

template <typename M>
struct isAbstractMarshaller<M,
        std::enable_if_t<
            ipg::isIpgMarshaller<M>::value &
            !std::is_same<M, ipg::IpgMarshaller<typename ipg::isIpgMarshaller<M>::inputType, typename ipg::isIpgMarshaller<M>::outputType>>::value>> {
    static constexpr bool value = true;
    using inputType = typename ipg::isIpgMarshaller<M>::inputType;
    using outputType = typename ipg::isIpgMarshaller<M>::outputType;
};

}

}

#endif
