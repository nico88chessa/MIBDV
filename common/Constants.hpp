#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <configure.h>

namespace PROGRAM_NAMESPACE {

using real = float;
using analogicReal = double;

enum class DeviceKey : int {
    NONE = -1,
    GALIL_CN = 1,
    GALIL_PLC
};

//enum class DeviceType {
//    CN,
//    PLC,
//};

enum class IOType : int {
    NOT_VALID = -1,
    POWER = 1,                              // digital input
    CYCLE,                                  // digital input
    EMERGENCY_MUSHROOM,                     // digital input
    DOOR,                                   // digital input
    BYPASS_SECURITY,                        // digital input
    WATER,                                  // digital input
    MARK_IN_PROGRESS,                       // digital input
    SCANNER_READY,                          // digital input
    SCANNER_ERROR,                          // digital input
    GENERIC_INPUT,                          // generic input
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
    GREEN_LIGHT                             // digital output
//    ANAL_AUTOFOCUS_DISTANCE,                // analogic input generic
//    ANAL_SUCTION_DEPRESSURE,                // analogic input generic
//    ANAL_AIR_PRESSURE                       // analogic input generic
};

//inline bool operator<(const IOType left, const IOType right) {
//    return static_cast<int>(left) < static_cast<int>(right);
//}

constexpr int PROGRAM_ERR_START_CODE = 0x01 << 16;

namespace settings {

// variabili di configurazione
constexpr char AXIS_X_STEP_PER_MM[] = "AxisX/StepPerMm";
constexpr char AXIS_X_MIN_POS_MM[] = "AxisX/MinPosMm";
constexpr char AXIS_X_MAX_POS_MM[] = "AxisX/MaxPosMm";
constexpr char AXIS_X_BASE_OFFSET_MM[] = "AxisX/BaseOffsetMm";
constexpr char AXIS_X_MANUAL_SPEED_MMS[] = "AxisX/ManualSpeedMms";
constexpr char AXIS_X_MANUAL_ACC_MMS2[] = "AxisX/ManualAccMms2";
constexpr char AXIS_X_MANUAL_DEC_MMS2[] = "AxisX/ManualDecMms2";
constexpr char AXIS_X_OPERATIVE_SPEED_MMS[] = "AxisX/OperativeSpeedMms";
constexpr char AXIS_X_OPERATIVE_ACC_MMS2[] = "AxisX/OperativeAccMms2";
constexpr char AXIS_X_OPERATIVE_DEC_MMS2[] = "AxisX/OperativeDecMms2";

constexpr char AXIS_Y_STEP_PER_MM[] = "AxisY/StepPerMm";
constexpr char AXIS_Y_MIN_POS_MM[] = "AxisY/MinPosMm";
constexpr char AXIS_Y_MAX_POS_MM[] = "AxisY/MaxPosMm";
constexpr char AXIS_Y_BASE_OFFSET_MM[] = "AxisY/BaseOffsetMm";
constexpr char AXIS_Y_MANUAL_SPEED_MMS[] = "AxisY/ManualSpeedMms";
constexpr char AXIS_Y_MANUAL_ACC_MMS2[] = "AxisY/ManualAccMms2";
constexpr char AXIS_Y_MANUAL_DEC_MMS2[] = "AxisY/ManualDecMms2";
constexpr char AXIS_Y_OPERATIVE_SPEED_MMS[] = "AxisY/OperativeSpeedMms";
constexpr char AXIS_Y_OPERATIVE_ACC_MMS2[] = "AxisY/OperativeAccMms2";
constexpr char AXIS_Y_OPERATIVE_DEC_MMS2[] = "AxisY/OperativeDecMms2";

constexpr char AXIS_Z_STEP_PER_MM[] = "AxisZ/StepPerMm";
constexpr char AXIS_Z_MIN_POS_MM[] = "AxisZ/MinPosMm";
constexpr char AXIS_Z_MAX_POS_MM[] = "AxisZ/MaxPosMm";
constexpr char AXIS_Z_BASE_OFFSET_MM[] = "AxisZ/BaseOffsetMm";
constexpr char AXIS_Z_MANUAL_SPEED_MMS[] = "AxisZ/ManualSpeedMms";
constexpr char AXIS_Z_MANUAL_ACC_MMS2[] = "AxisZ/ManualAccMms2";
constexpr char AXIS_Z_MANUAL_DEC_MMS2[] = "AxisZ/ManualDecMms2";
constexpr char AXIS_Z_OPERATIVE_SPEED_MMS[] = "AxisZ/OperativeSpeedMms";
constexpr char AXIS_Z_OPERATIVE_ACC_MMS2[] = "AxisZ/OperativeAccMms2";
constexpr char AXIS_Z_OPERATIVE_DEC_MMS2[] = "AxisZ/OperativeDecMms2";

constexpr char DIGITAL_INPUT_TYPE[] = "Type";
constexpr char DIGITAL_INPUT_NAME[] = "Name";
constexpr char DIGITAL_INPUT_CHANNEL[] = "Channel";
constexpr char DIGITAL_INPUT_INVERT_LOGIC[] = "InvertLogic";
constexpr char DIGITAL_INPUT_DEVICE[] = "Device";
constexpr char DIGITAL_INPUT_IS_ALARM[] = "IsAlarm";

constexpr char DIGITAL_OUTPUT_TYPE[] = "Type";
constexpr char DIGITAL_OUTPUT_NAME[] = "Name";
constexpr char DIGITAL_OUTPUT_CHANNEL[] = "Channel";
constexpr char DIGITAL_OUTPUT_INVERT_LOGIC[] = "InvertLogic";
constexpr char DIGITAL_OUTPUT_DEVICE[] = "Device";
constexpr char DIGITAL_OUTPUT_IS_ALARM[] = "IsAlarm";

constexpr char ANALOGC_INPUT_TYPE[] = "Type";
constexpr char ANALOGC_INPUT_NAME[] = "Name";
constexpr char ANALOGC_INPUT_CHANNEL[] = "Channel";
constexpr char ANALOGC_INPUT_DEVICE[] = "Device";
constexpr char ANALOGC_INPUT_IS_ALARM[] = "IsAlarm";
constexpr char ANALOGC_INPUT_GAIN[] = "Gain";
constexpr char ANALOGC_INPUT_OFFSET[] = "Offset";
constexpr char ANALOGC_INPUT_UNIT[] = "Unit";
constexpr char ANALOGC_INPUT_LOWER_LIMIT[] = "LowerLimit";
constexpr char ANALOGC_INPUT_UPPER_LIMIT[] = "UpperLimit";
constexpr char ANALOGC_INPUT_HYSTERESIS[] = "Hysteresis";

constexpr int AXIS_X_STEP_PER_MM_DFLT = 1;
constexpr int AXIS_X_MIN_POS_MM_DFLT = 2;
constexpr int AXIS_X_MAX_POS_MM_DFLT = 3;
constexpr real AXIS_X_BASE_OFFSET_MM_DFLT = 4;
constexpr real AXIS_X_MANUAL_SPEED_MMS_DFLT = 5;
constexpr real AXIS_X_MANUAL_ACC_MMS2_DFLT = 6;
constexpr real AXIS_X_MANUAL_DEC_MMS2_DFLT = 7;
constexpr real AXIS_X_OPERATIVE_SPEED_MMS_DFLT = 8;
constexpr real AXIS_X_OPERATIVE_ACC_MMS2_DFLT = 9;
constexpr real AXIS_X_OPERATIVE_DEC_MMS2_DFLT = 10;

constexpr int AXIS_Y_STEP_PER_MM_DFLT = 12;
constexpr int AXIS_Y_MIN_POS_MM_DFLT = 13;
constexpr int AXIS_Y_MAX_POS_MM_DFLT = 14;
constexpr real AXIS_Y_BASE_OFFSET_MM_DFLT = 15;
constexpr real AXIS_Y_MANUAL_SPEED_MMS_DFLT = 16;
constexpr real AXIS_Y_MANUAL_ACC_MMS2_DFLT = 17;
constexpr real AXIS_Y_MANUAL_DEC_MMS2_DFLT = 18;
constexpr real AXIS_Y_OPERATIVE_SPEED_MMS_DFLT = 19;
constexpr real AXIS_Y_OPERATIVE_ACC_MMS2_DFLT = 20;
constexpr real AXIS_Y_OPERATIVE_DEC_MMS2_DFLT = 21;

constexpr int AXIS_Z_STEP_PER_MM_DFLT = 23;
constexpr int AXIS_Z_MIN_POS_MM_DFLT = 24;
constexpr int AXIS_Z_MAX_POS_MM_DFLT = 25;
constexpr real AXIS_Z_BASE_OFFSET_MM_DFLT = 26;
constexpr real AXIS_Z_MANUAL_SPEED_MMS_DFLT = 27;
constexpr real AXIS_Z_MANUAL_ACC_MMS2_DFLT = 28;
constexpr real AXIS_Z_MANUAL_DEC_MMS2_DFLT = 29;
constexpr real AXIS_Z_OPERATIVE_SPEED_MMS_DFLT = 30;
constexpr real AXIS_Z_OPERATIVE_ACC_MMS2_DFLT = 31;
constexpr real AXIS_Z_OPERATIVE_DEC_MMS2_DFLT = 32;

}

}

#endif // CONSTANTS_HPP
