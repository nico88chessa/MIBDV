#ifndef COMMONTRAITS_HPP
#define COMMONTRAITS_HPP

#include "Constants.hpp"

namespace PROGRAM_NAMESPACE {

class GalilCNController;
class GalilPLCController;

template <DeviceKey>
struct deviceKeyTraits {
    static constexpr bool value = false;
    using type = int;
};

template <>
struct deviceKeyTraits<DeviceKey::GALIL_CN> {
    static constexpr bool value = true;
    using type = GalilCNController;
};

template <>
struct deviceKeyTraits<DeviceKey::GALIL_PLC> {
    static constexpr bool value = true;
    using type = GalilPLCController;
};

}


#endif // COMMONTRAITS_HPP
