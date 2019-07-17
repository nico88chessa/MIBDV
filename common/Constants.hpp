#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <configure.h>
#include <Types.hpp>

namespace PROGRAM_NAMESPACE {

constexpr double EPSILON = 2.0;

constexpr int DIGITAL_INPUT_CHANNEL_NONE = -1;
constexpr int DIGITAL_OUTPUT_CHANNEL_NONE = -1;
constexpr int ANALOG_INPUT_CHANNEL_NONE = -1;
constexpr int ANALOG_OUTPUT_CHANNEL_NONE = -1;

constexpr int PROGRAM_ERR_START_CODE = 0x01 << 16; // 65536

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
constexpr char AXIS_X_HOMING_SPEED_MMS[] = "AxisX/HomingSpeedMms";
constexpr char AXIS_X_HOMING_ACC_MMS2[] = "AxisX/HomingAccMms2";
constexpr char AXIS_X_HOMING_DEC_MMS2[] = "AxisX/HomingDecMms2";
constexpr char AXIS_X_CHECK_LIMITS[] = "AxisX/CheckLimits";

constexpr int AXIS_X_STEP_PER_MM_DFLT = 16385;
constexpr int AXIS_X_MIN_POS_MM_DFLT = 0;
constexpr int AXIS_X_MAX_POS_MM_DFLT = 700;
constexpr real AXIS_X_BASE_OFFSET_MM_DFLT = 0;
constexpr real AXIS_X_MANUAL_SPEED_MMS_DFLT = 10;
constexpr real AXIS_X_MANUAL_ACC_MMS2_DFLT = 10;
constexpr real AXIS_X_MANUAL_DEC_MMS2_DFLT = 10;
constexpr real AXIS_X_OPERATIVE_SPEED_MMS_DFLT = 10;
constexpr real AXIS_X_OPERATIVE_ACC_MMS2_DFLT = 10;
constexpr real AXIS_X_OPERATIVE_DEC_MMS2_DFLT = 10;
constexpr real AXIS_X_HOMING_SPEED_MMS_DFLT = AXIS_X_MANUAL_SPEED_MMS_DFLT;
constexpr real AXIS_X_HOMING_ACC_MMS2_DFLT = AXIS_X_MANUAL_ACC_MMS2_DFLT;
constexpr real AXIS_X_HOMING_DEC_MMS2_DFLT = AXIS_X_MANUAL_DEC_MMS2_DFLT;
constexpr bool AXIS_X_CHECK_LIMITS_DFLT = true;

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
constexpr char AXIS_Y_HOMING_SPEED_MMS[] = "AxisY/HomingSpeedMms";
constexpr char AXIS_Y_HOMING_ACC_MMS2[] = "AxisY/HomingAccMms2";
constexpr char AXIS_Y_HOMING_DEC_MMS2[] = "AxisY/HomingDecMms2";
constexpr char AXIS_Y_CHECK_LIMITS[] = "AxisY/CheckLimits";

constexpr int AXIS_Y_STEP_PER_MM_DFLT = 1000;
constexpr int AXIS_Y_MIN_POS_MM_DFLT = 0;
constexpr int AXIS_Y_MAX_POS_MM_DFLT = 700;
constexpr real AXIS_Y_BASE_OFFSET_MM_DFLT = 0;
constexpr real AXIS_Y_MANUAL_SPEED_MMS_DFLT = 10;
constexpr real AXIS_Y_MANUAL_ACC_MMS2_DFLT = 10;
constexpr real AXIS_Y_MANUAL_DEC_MMS2_DFLT = 10;
constexpr real AXIS_Y_OPERATIVE_SPEED_MMS_DFLT = 10;
constexpr real AXIS_Y_OPERATIVE_ACC_MMS2_DFLT = 10;
constexpr real AXIS_Y_OPERATIVE_DEC_MMS2_DFLT = 10;
constexpr real AXIS_Y_HOMING_SPEED_MMS_DFLT = AXIS_Y_MANUAL_SPEED_MMS_DFLT;
constexpr real AXIS_Y_HOMING_ACC_MMS2_DFLT = AXIS_Y_MANUAL_ACC_MMS2_DFLT;
constexpr real AXIS_Y_HOMING_DEC_MMS2_DFLT = AXIS_Y_MANUAL_DEC_MMS2_DFLT;
constexpr bool AXIS_Y_CHECK_LIMITS_DFLT = true;

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
constexpr char AXIS_Z_HOMING_SPEED_MMS[] = "AxisZ/HomingSpeedMms";
constexpr char AXIS_Z_HOMING_ACC_MMS2[] = "AxisZ/HomingAccMms2";
constexpr char AXIS_Z_HOMING_DEC_MMS2[] = "AxisZ/HomingDecMms2";
constexpr char AXIS_Z_CHECK_LIMITS[] = "AxisZ/CheckLimits";

constexpr int AXIS_Z_STEP_PER_MM_DFLT = 200;
constexpr int AXIS_Z_MIN_POS_MM_DFLT = 0;
constexpr int AXIS_Z_MAX_POS_MM_DFLT = 200;
constexpr real AXIS_Z_BASE_OFFSET_MM_DFLT = 0;
constexpr real AXIS_Z_MANUAL_SPEED_MMS_DFLT = 10;
constexpr real AXIS_Z_MANUAL_ACC_MMS2_DFLT = 10;
constexpr real AXIS_Z_MANUAL_DEC_MMS2_DFLT = 10;
constexpr real AXIS_Z_OPERATIVE_SPEED_MMS_DFLT = 10;
constexpr real AXIS_Z_OPERATIVE_ACC_MMS2_DFLT = 10;
constexpr real AXIS_Z_OPERATIVE_DEC_MMS2_DFLT = 10;
constexpr real AXIS_Z_HOMING_SPEED_MMS_DFLT = AXIS_Z_MANUAL_SPEED_MMS_DFLT;
constexpr real AXIS_Z_HOMING_ACC_MMS2_DFLT = AXIS_Z_MANUAL_ACC_MMS2_DFLT;
constexpr real AXIS_Z_HOMING_DEC_MMS2_DFLT = AXIS_Z_MANUAL_DEC_MMS2_DFLT;
constexpr bool AXIS_Z_CHECK_LIMITS_DFLT = true;

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

constexpr char ANALOG_INPUT_NAME[] = "Name";
constexpr char ANALOG_INPUT_CHANNEL[] = "Channel";
constexpr char ANALOG_INPUT_DEVICE[] = "Device";
constexpr char ANALOG_INPUT_IS_ALARM[] = "IsAlarm";
constexpr char ANALOG_INPUT_GAIN[] = "Gain";
constexpr char ANALOG_INPUT_OFFSET[] = "Offset";
constexpr char ANALOG_INPUT_UNIT[] = "Unit";
constexpr char ANALOG_INPUT_LOWER_LIMIT[] = "LowerLimit";
constexpr char ANALOG_INPUT_UPPER_LIMIT[] = "UpperLimit";
constexpr char ANALOG_INPUT_HYSTERESIS[] = "Hysteresis";

constexpr char MACHINE_CN_TYPE[] = "Machine/CNType";
constexpr char MACHINE_PLC_TYPE[] = "Machine/PLCType";
constexpr char MACHINE_IO_REFRESH_INTERVAL_MS[] = "Machine/IORefreshIntervalMs";

constexpr DeviceKey MACHINE_CN_TYPE_DFLT = DeviceKey::NONE;
constexpr DeviceKey MACHINE_PLC_TYPE_DFLT = DeviceKey::NONE;
constexpr int MACHINE_IO_REFRESH_INTERVAL_MS_DFLT = 1000;

constexpr char UI_PREFIX[] = "Ui";
constexpr char UI_SPOOL_PATH[] = "Ui/SpoolPath";

constexpr const char* UI_SPOOL_PATH_DFLT = PROGRAM_NAMESPACE::SPOOL_PATH_DFLT;

constexpr char GALIL_CN_PREFIX[] = "GalilCN";
constexpr char GALIL_CN_CONNECTION_TIMEOUT_MS[] = "GalilCN/ConnectionTimeoutMs";
constexpr char GALIL_CN_STATUS_REFRESH_INTERVAL_MS[] = "GalilCN/StatusRefreshIntervalMs";
constexpr char GALIL_CN_CHECK_CONNECTION_INTERVAL_MS[] = "GalilCN/CheckConnectionIntervalMs";
constexpr char GALIL_CN_RECONNECTION_INTERVAL_MS[] = "GalilCN/ReconnectionIntervalMs";
constexpr char GALIL_CN_NUMBER_DIGITAL_INPUT[] = "GalilCN/NumberDigitalInput";
constexpr char GALIL_CN_NUMBER_DIGITAL_OUTPUT[] = "GalilCN/NumberDigitalOutput";
constexpr char GALIL_CN_NUMBER_ANALOG_INPUT[] = "GalilCN/NumberAnalogcInput";
constexpr char GALIL_CN_NUMBER_ANALOG_OUTPUT[] = "GalilCN/NumberAnalogcOutput";
constexpr char GALIL_CN_IP_ADDRESS[] = "GalilCN/IpAddress";
constexpr char GALIL_CN_OPTION_CUSTOM_HOME_AXIS_X[] = "GalilCN/Option/CustomHomeAxisX";

constexpr int GALIL_CN_CONNECTION_TIMEOUT_MS_DFLT = 1000;
constexpr int GALIL_CN_STATUS_REFRESH_INTERVAL_MS_DFLT = 100;
constexpr int GALIL_CN_CHECK_CONNECTION_INTERVAL_MS_DFLT = 1000;
constexpr int GALIL_CN_RECONNECTION_INTERVAL_MS_DFLT = 10000;
constexpr int GALIL_CN_NUMBER_DIGITAL_INPUT_DFLT = 8;
constexpr int GALIL_CN_NUMBER_DIGITAL_OUTPUT_DFLT = 8;
constexpr int GALIL_CN_NUMBER_ANALOG_INPUT_DFLT = 0;
constexpr int GALIL_CN_NUMBER_ANALOG_OUTPUT_DFLT = 0;
constexpr char GALIL_CN_IP_ADDRESS_DFLT[] = "169.254.12.10";
constexpr bool GALIL_CN_OPTION_CUSTOM_HOME_AXIS_X_DFLT = false;

constexpr char GALIL_PLC_PREFIX[] = "GalilPLC";
constexpr char GALIL_PLC_CONNECTION_TIMEOUT_MS[] = "GalilPLC/ConnectionTimeoutMs";
constexpr char GALIL_PLC_STATUS_REFRESH_INTERVAL_MS[] = "GalilPLC/StatusRefreshIntervalMs";
constexpr char GALIL_PLC_CHECK_CONNECTION_INTERVAL_MS[] = "GalilPLC/CheckConnectionIntervalMs";
constexpr char GALIL_PLC_RECONNECTION_INTERVAL_MS[] = "GalilPLC/ReconnectionIntervalMs";
constexpr char GALIL_PLC_NUMBER_DIGITAL_INPUT[] = "GalilPLC/NumberDigitalInput";
constexpr char GALIL_PLC_NUMBER_DIGITAL_OUTPUT[] = "GalilPLC/NumberDigitalOutput";
constexpr char GALIL_PLC_NUMBER_ANALOG_INPUT[] = "GalilPLC/NumberAnalogcInput";
constexpr char GALIL_PLC_NUMBER_ANALOG_OUTPUT[] = "GalilPLC/NumberAnalogcOutput";
constexpr char GALIL_PLC_IP_ADDRESS[] = "GalilPLC/IpAddress";

constexpr int GALIL_PLC_CONNECTION_TIMEOUT_MS_DFLT = 1000;
constexpr int GALIL_PLC_STATUS_REFRESH_INTERVAL_MS_DFLT = 100;
constexpr int GALIL_PLC_CHECK_CONNECTION_INTERVAL_MS_DFLT = 10000;
constexpr int GALIL_PLC_RECONNECTION_INTERVAL_MS_DFLT = 10000;
constexpr int GALIL_PLC_NUMBER_DIGITAL_INPUT_DFLT = 16;
constexpr int GALIL_PLC_NUMBER_DIGITAL_OUTPUT_DFLT = 16;
constexpr int GALIL_PLC_NUMBER_ANALOG_INPUT_DFLT = 8;
constexpr int GALIL_PLC_NUMBER_ANALOG_OUTPUT_DFLT = 0;
constexpr char GALIL_PLC_IP_ADDRESS_DFLT[] = "169.254.12.11";


}

}

#endif // CONSTANTS_HPP
