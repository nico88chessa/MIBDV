#ifndef TYPES_HPP
#define TYPES_HPP

// type traits
#include <QMetaType>
#include <QMap>
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
class GalilCNInspector;
class GalilPLCController;
class GalilPLCInspector;
class DigitalInput;
class DigitalOutput;
class AnalogInput;
class DigitalInputValue;
class DigitalOutputValue;
class AnalogInputValue;
class AnalogInputBufferValues;
class IOSignaler;
class MotionAnalizer;
class DeviceConnectionWatcher;

using ErrorID = int;


enum class ErrorType : int {
    INFO = 0,
    WARNING,
    ERROR,
    FATAL
};

enum class DeviceKey : int {
    NONE = -1,
    GALIL_CN = 0,
    GALIL_CN_INSPECTOR,
    GALIL_PLC,
    GALIL_PLC_INSPECTOR,
    IO_SIGNALER,
    MOTION_ANALIZER,
    DEVICE_CONNECTION_WATCHER
};

enum class IOType : int {
    NOT_VALID = -1,
    POWER = 0,                              // digital input
    CYCLE,                                  // digital input
    EMERGENCY_MUSHROOM,                     // digital input
    DOOR_OPEN,                              // digital input
    MAINTENANCE,                            // digital input
    WATER_ALARM,                            // digital input
    MARK_IN_PROGRESS,                       // digital input
    SCANNER_READY,                          // digital input
    SCANNER_ERROR,                          // digital input
    DISTANCE_SENSOR_FAULT,                  // digital input
    DEPRESSURE_SENSOR_FAULT,                // digital input
    PRESSURE_1_SENSOR_FAULT,                // digital input
    PRESSURE_2_SENSOR_FAULT,                // digital input
    GENERIC_INPUT,                          // generic digital input
    LASER_POWER,                            // digital output
    COMPRESSED_AIR_1,                       // digital output
    COMPRESSED_AIR_2,                       // digital output
    SUCTION,                                // digital output
    POWER_SCAN,                             // digital output
    START_SCAN,                             // digital output
    STOP_SCAN,                              // digital output
    YELLOW_LIGHT,                           // digital output
    RED_LIGHT,                              // digital output
    GREEN_LIGHT,                            // digital output
    GENERIC_ANALOG_INPUT,                   // generic analog input
};

enum class MotionStopCode : int {
    MOTION_NAN,
    MOTION_IS_RUNNING,
    MOTION_STOP_CORRECTLY,
    MOTION_STOP_COMMAND,
    MOTION_STOP_ON_ERROR
};

enum class MachineStatus : int {
    STATUS_NAN = -1,
    IDLE,
    STOP_RESUMABLE,
    PAUSE,
    PRINTING
};


using DigitalInputSet = QMap<IOType, DigitalInput>;
using DigitalOutputSet = QMap<IOType, DigitalOutput>;
using AnalogInputSet = QMap<IOType, AnalogInput>;

using DigitalInputStatus = QMap<IOType, DigitalInputValue>;
using DigitalOutputStatus = QMap<IOType, DigitalOutputValue>;
using AnalogInputStatus = QMap<IOType, AnalogInputValue>;
using AnalogInputBufferStatus = QMap<IOType, AnalogInputBufferValues>;

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
struct deviceKeyTraits<DeviceKey::GALIL_CN_INSPECTOR> {
    static constexpr bool value = true;
    using type = GalilCNInspector;
};

template <>
struct deviceKeyTraits<DeviceKey::GALIL_PLC> {
    static constexpr bool value = true;
    using type = GalilPLCController;
};

template <>
struct deviceKeyTraits<DeviceKey::IO_SIGNALER> {
    static constexpr bool value = true;
    using type = IOSignaler;
};

template <>
struct deviceKeyTraits<DeviceKey::MOTION_ANALIZER> {
    static constexpr bool value = true;
    using type = MotionAnalizer;
};

template <>
struct deviceKeyTraits<DeviceKey::DEVICE_CONNECTION_WATCHER> {
    static constexpr bool value = true;
    using type = DeviceConnectionWatcher;
};

template <typename T>
constexpr DeviceKey getDeviceKeyFromTemplate() {
    return std::is_same_v<T, GalilCNController> ? DeviceKey::GALIL_CN :
           std::is_same_v<T, GalilCNInspector> ? DeviceKey::GALIL_CN_INSPECTOR :
           std::is_same_v<T, GalilPLCController> ? DeviceKey::GALIL_PLC :
           std::is_same_v<T, GalilPLCInspector> ? DeviceKey::GALIL_PLC_INSPECTOR :
           std::is_same_v<T, IOSignaler> ? DeviceKey::IO_SIGNALER :
           std::is_same_v<T, MotionAnalizer> ? DeviceKey::MOTION_ANALIZER :
           std::is_same_v<T, DeviceConnectionWatcher> ? DeviceKey::DEVICE_CONNECTION_WATCHER :
           DeviceKey::NONE;
}

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::ErrorType)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DeviceKey)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::IOType)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::MotionStopCode)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::MachineStatus)

#endif // TYPES_HPP
