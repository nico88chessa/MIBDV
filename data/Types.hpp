#ifndef TYPES_HPP
#define TYPES_HPP

// type traits
#include <configure.h>

namespace PROGRAM_NAMESPACE {

using real = float;
using realHR = double; // real high resolution
using analogReal = double;

using posCNType = int; // sono impulsi, quindi uso un numero intero
using spdCNType = int; // sono impulsi, quindi uso un numero intero
using accCNType = int; // sono impulsi, quindi uso un numero intero

using anlType = double;

using posType = float;

class GalilCNController;
class GalilPLCController;

enum class DeviceKey : int {
    NONE = -1,
    GALIL_CN = 0,
    GALIL_PLC
};

enum class IOType : int {
    NOT_VALID = -1,
    POWER = 0,                              // digital input
    CYCLE,                                  // digital input
    EMERGENCY_MUSHROOM,                     // digital input
    DOOR,                                   // digital input
    BYPASS_SECURITY,                        // digital input
    WATER,                                  // digital input
    MARK_IN_PROGRESS,                       // digital input
    SCANNER_READY,                          // digital input
    SCANNER_ERROR,                          // digital input
    GENERIC_INPUT,                          // generic digital input
    LASER_POWER,                            // digital output
    COMPRESSED_AIR_1,                       // digital output
    COMPRESSED_AIR_2,                       // digital output
    BRUSH_1,                                // digital output
    BRUSH_2,                                // digital output
    SUCTION,                                // digital output
    ENABLE_AIR_FOR_FLUID,                   // digital output
    ENABLE_FLUID,                           // digital output
    POWER_SCAN,                             // digital output
    START_SCAN,                             // digital output
    STOP_SCAN,                              // digital output
    RED_LIGHT,                              // digital output
    GREEN_LIGHT,                            // digital output
    GENERIC_ANALOG_INPUT,                   // generic analog input
};

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

#endif // TYPES_HPP
