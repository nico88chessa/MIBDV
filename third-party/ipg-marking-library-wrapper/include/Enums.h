#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    enum class Optimization : int {
        DEFAULT,
        QUALITY,
        SPEED
    };

    enum class ConnectionStatus : int {
        AVAILABLE,
        BUSY
    };

    enum class SortType : int {
        POLYLINE,
        START_PROXIMITY
    };

    enum class Units : int {
        INCHES,
        METERS,
        MICRONS,
        MILLIMETERS
    };

    enum class WaitEvent : int {
        GPI0,
        GPI1,
        GPI2,
        GPI3,
        StartBit
    };

    enum class LaserAction : int {
        Disable,
        Enable
    };

}