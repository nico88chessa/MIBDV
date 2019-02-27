#ifndef IPG_CONSTANTS_HPP
#define IPG_CONSTANTS_HPP

#include "Datatype.hpp"

namespace ipg {

//static constexpr int CONNECTION_DEFAULT_PORT = 80;
//static constexpr char CONNECTION_DEFAULT_IP[] = "192.168.1.1";

static constexpr int CONNECTION_DEFAULT_TIMEOUT_MS = 500;
static constexpr int CONNECTION_MAX_WRITE_TIME_MS = 500;
static constexpr int CONNECTION_MAX_READ_TIME_MS = 500;

// main interface
static constexpr int MAIN_INTERFACE_RS232 = 1;
static constexpr int MAIN_INTERFACE_ETHERNET = 2;
static constexpr int MAIN_INTERFACE_PARALLEL_IO = 3;
static constexpr int MAIN_INTERFACE_FIELDBUS = 4;
static constexpr int MAIN_INTERFACE_FIELDS_NUM = 4;

static const char* MAIN_INTERFACE_STRING_LIST[MAIN_INTERFACE_FIELDS_NUM] = {
    "RS-232",
    "ETHERNET",
    "PARALLEL",
    "FIELDBUS",
};

// interface config
static constexpr int INTERFACE_CONFIG_RS232 = 1;
static constexpr int INTERFACE_CONFIG_ETHERNET = 2;
static constexpr int INTERFACE_CONFIG_PARALLEL_IO = 3;
static constexpr int INTERFACE_CONFIG_FIELDBUS = 4;
static constexpr int INTERFACE_CONFIG_HW = 5;
static constexpr int INTERFACE_CONFIG_AUXILIARY = 6;
static constexpr int INTERFACE_CONFIG_MAIN = 9;
static constexpr int INTERFACE_CONFIG_FIELDS_NUM = 7;

static const char* INTERFACE_CONFIG_STRING_LIST[INTERFACE_CONFIG_FIELDS_NUM] = {
    "RS-232",
    "ETHERNET",
    "PARALLEL",
    "FIELDBUS",
    "HW",
    "AUXILIARY",
    "MAIN",
};

/***** bit flag usati dal metodo laser status *****/
// main status
static constexpr int MAIN_STATUS_ALARMS_BIT = 0;
static constexpr int MAIN_STATUS_WARNINGS_BIT = 1;

static constexpr int MAIN_STATUS_ALARMS = 0x01 << MAIN_STATUS_ALARMS_BIT;
static constexpr int MAIN_STATUS_WARNINGS = 0x01 << MAIN_STATUS_WARNINGS_BIT;

static constexpr int INSTALLED_STATUS_POWER_SUPPLY_BIT = 0;
static constexpr int INSTALLED_STATUS_EMISSION_BIT = 1;
static constexpr int INSTALLED_STATUS_MODULATION_BIT = 2;
static constexpr int INSTALLED_STATUS_GUIDE_LASER_BIT = 3;

static constexpr int INSTALLED_STATUS_POWER_SUPPLY = 0x01 << INSTALLED_STATUS_POWER_SUPPLY_BIT;
static constexpr int INSTALLED_STATUS_EMISSION = 0x01 << INSTALLED_STATUS_EMISSION_BIT;
static constexpr int INSTALLED_STATUS_MODULATION = 0x01 << INSTALLED_STATUS_MODULATION_BIT;
static constexpr int INSTALLED_STATUS_GUIDE_LASER = 0x01 << INSTALLED_STATUS_GUIDE_LASER_BIT;


// bit alarm
static constexpr int ALARM_SYSTEM_ALARM_BIT = 0;
static constexpr int ALARM_CRITICAL_ALARM_BIT = 1;
static constexpr int ALARM_TEMPERATURE_ALARM_BIT = 2;
static constexpr int ALARM_INTERFACE_CONTROLLER_24V_ALARM_BIT = 3;
static constexpr int ALARM_BACK_REFLECTION_ALARM_BIT = 4;
static constexpr int ALARM_OPTICAL_LINK_ALARM_BIT = 5;
static constexpr int ALARM_PROCESS_FIBER_ALARM_BIT = 6;

static constexpr int ALARM_SYSTEM_ALARM = 0x01 << ALARM_SYSTEM_ALARM_BIT;
static constexpr int ALARM_CRITICAL_ALARM = 0x01 << ALARM_CRITICAL_ALARM_BIT;
static constexpr int ALARM_TEMPERATURE_ALARM = 0x01 << ALARM_TEMPERATURE_ALARM_BIT;
static constexpr int ALARM_INTERFACE_CONTROLLER_24V_ALARM = 0x01 << ALARM_INTERFACE_CONTROLLER_24V_ALARM_BIT;
static constexpr int ALARM_BACK_REFLECTION_ALARM = 0x01 << ALARM_BACK_REFLECTION_ALARM_BIT;
static constexpr int ALARM_OPTICAL_LINK_ALARM = 0x01 << ALARM_OPTICAL_LINK_ALARM_BIT;
static constexpr int ALARM_PROCESS_FIBER_ALARM = 0x01 << ALARM_PROCESS_FIBER_ALARM_BIT;

// bit system alarm
static constexpr int SYSTEM_ALARM_LASER_INTERFACE_BIT = 0;
static constexpr int SYSTEM_ALARM_OPTICAL_MODULE_1_BIT = 1;
static constexpr int SYSTEM_ALARM_OPTICAL_MODULE_2_BIT = 2;
static constexpr int SYSTEM_ALARM_POWER_SUPPLY_BIT = 3;
static constexpr int SYSTEM_ALARM_SAFETY_BIT = 4;
static constexpr int SYSTEM_ALARM_COMMUNICATION_OM1_BIT = 5;
static constexpr int SYSTEM_ALARM_COMMUNICATION_OM2_BIT = 6;
static constexpr int SYSTEM_ALARM_COMMUNICATION_PS_BIT = 7;
static constexpr int SYSTEM_ALARM_COMMUNICATION_SAFETY_BIT = 8;
static constexpr int SYSTEM_ALARM_FEEDING_FIBER_ALARM_BIT = 9;

static constexpr int SYSTEM_ALARM_LASER_INTERFACE = 0x01 << SYSTEM_ALARM_LASER_INTERFACE_BIT;
static constexpr int SYSTEM_ALARM_OPTICAL_MODULE_1 = 0x01 << SYSTEM_ALARM_OPTICAL_MODULE_1_BIT;
static constexpr int SYSTEM_ALARM_OPTICAL_MODULE_2 = 0x01 << SYSTEM_ALARM_OPTICAL_MODULE_2_BIT;
static constexpr int SYSTEM_ALARM_POWER_SUPPLY = 0x01 << SYSTEM_ALARM_POWER_SUPPLY_BIT;
static constexpr int SYSTEM_ALARM_SAFETY = 0x01 << SYSTEM_ALARM_SAFETY_BIT;
static constexpr int SYSTEM_ALARM_COMMUNICATION_OM1 = 0x01 << SYSTEM_ALARM_COMMUNICATION_OM1_BIT;
static constexpr int SYSTEM_ALARM_COMMUNICATION_OM2 = 0x01 << SYSTEM_ALARM_COMMUNICATION_OM2_BIT;
static constexpr int SYSTEM_ALARM_COMMUNICATION_PS = 0x01 << SYSTEM_ALARM_COMMUNICATION_PS_BIT;
static constexpr int SYSTEM_ALARM_COMMUNICATION_SAFETY = 0x01 << SYSTEM_ALARM_COMMUNICATION_SAFETY_BIT;
static constexpr int SYSTEM_ALARM_FEEDING_FIBER_ALARM = 0x01 << SYSTEM_ALARM_FEEDING_FIBER_ALARM_BIT;

// bit extended alarm
static constexpr int EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_LOW_BIT = 0;
static constexpr int EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_HIGH_BIT = 1;
static constexpr int EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_LOW_BIT = 2;
static constexpr int EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_HIGH_BIT = 3;
static constexpr int EXTENDED_ALARM_LASER_TEMPERATURE_TOO_LOW_BIT = 4;
static constexpr int EXTENDED_ALARM_LASER_TEMPERATURE_TOO_HIGH_BIT = 5;
static constexpr int EXTENDED_ALARM_PS_TEMPERATURE_TOO_HIGH_BIT = 8;

static constexpr int EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_LOW = 0x01 << EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_LOW_BIT;
static constexpr int EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_HIGH = 0x01 << EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_HIGH_BIT;
static constexpr int EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_LOW = 0x01 << EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_LOW_BIT;
static constexpr int EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_HIGH = 0x01 << EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_HIGH_BIT;
static constexpr int EXTENDED_ALARM_LASER_TEMPERATURE_TOO_LOW = 0x01 << EXTENDED_ALARM_LASER_TEMPERATURE_TOO_LOW_BIT;
static constexpr int EXTENDED_ALARM_LASER_TEMPERATURE_TOO_HIGH = 0x01 << EXTENDED_ALARM_LASER_TEMPERATURE_TOO_HIGH_BIT;
static constexpr int EXTENDED_ALARM_PS_TEMPERATURE_TOO_HIGH = 0x01 << EXTENDED_ALARM_PS_TEMPERATURE_TOO_HIGH_BIT;

// bit warnings
static constexpr int WARNINGS_PRR_LOW_BIT = 0;
static constexpr int WARNINGS_PRR_HIGH_BIT = 1;
static constexpr int WARNINGS_SDCARD_FAILURE_BIT = 2;
static constexpr int WARNINGS_INTERFACE_TEMPERATURE_TOO_LOW_BIT = 3;
static constexpr int WARNINGS_INTERFACE_TEMPERATURE_TOO_HIGH_BIT = 4;
static constexpr int WARNINGS_LASER_TEMPERATURE_TOO_LOW_BIT = 5;
static constexpr int WARNINGS_LASER_TEMPERATURE_TOO_HIGH_BIT = 6;

static constexpr int WARNINGS_PRR_LOW = 0x01 << WARNINGS_PRR_LOW_BIT;
static constexpr int WARNINGS_PRR_HIGH = 0x01 << WARNINGS_PRR_HIGH_BIT;
static constexpr int WARNINGS_SDCARD_FAILURE = 0x01 << WARNINGS_SDCARD_FAILURE_BIT;
static constexpr int WARNINGS_INTERFACE_TEMPERATURE_TOO_LOW = 0x01 << WARNINGS_INTERFACE_TEMPERATURE_TOO_LOW_BIT;
static constexpr int WARNINGS_INTERFACE_TEMPERATURE_TOO_HIGH = 0x01 << WARNINGS_INTERFACE_TEMPERATURE_TOO_HIGH_BIT;
static constexpr int WARNINGS_LASER_TEMPERATURE_TOO_LOW = 0x01 << WARNINGS_LASER_TEMPERATURE_TOO_LOW_BIT;
static constexpr int WARNINGS_LASER_TEMPERATURE_TOO_HIGH = 0x01 << WARNINGS_LASER_TEMPERATURE_TOO_HIGH_BIT;

// status laser
static constexpr int STATUS_SAFETY_CIRCUIT_OPEN_BIT = 0;
static constexpr int STATUS_POWER_SUPPLY_IS_ON_BIT = 1;
static constexpr int STATUS_LASER_READY_BIT = 2;
static constexpr int STATUS_EMISSION_IS_ON_BIT = 3;
static constexpr int STATUS_GUIDE_LASER_BIT = 4;
static constexpr int STATUS_EXTERNAL_STOP_BIT = 5;
static constexpr int STATUS_SAFETY_CIRCUIT_BLOCKED_BIT = 6;
static constexpr int STATUS_HOT_STOP_BIT = 7;

static constexpr int STATUS_SAFETY_CIRCUIT_OPEN = 0x01 << STATUS_SAFETY_CIRCUIT_OPEN_BIT;
static constexpr int STATUS_POWER_SUPPLY_IS_ON = 0x01 << STATUS_POWER_SUPPLY_IS_ON_BIT;
static constexpr int STATUS_LASER_READY = 0x01 << STATUS_LASER_READY_BIT;
static constexpr int STATUS_EMISSION_IS_ON = 0x01 << STATUS_EMISSION_IS_ON_BIT;
static constexpr int STATUS_GUIDE_LASER = 0x01 << STATUS_GUIDE_LASER_BIT;
static constexpr int STATUS_EXTERNAL_STOP = 0x01 << STATUS_EXTERNAL_STOP_BIT;
static constexpr int STATUS_SAFETY_CIRCUIT_BLOCKED = 0x01 << STATUS_SAFETY_CIRCUIT_BLOCKED_BIT;
static constexpr int STATUS_HOT_STOP = 0x01 << STATUS_HOT_STOP_BIT;

// bit extended status
static constexpr int EXTENDED_STATUS_INTERFACE_PS_HOT_STOP_BIT = 0;
static constexpr int EXTENDED_STATUS_MAIN_PS_HOT_STOP_BIT = 1;
static constexpr int EXTENDED_STATUS_OPTICAL_LINK_HOT_STOP_BIT = 2;
static constexpr int EXTENDED_STATUS_SAFETY_OPEN_WHILE_EMISSION_STOP_BIT = 3;
static constexpr int EXTENDED_STATUS_EXTERNAL_STOP_WHILE_EMISSION_BIT = 4;
static constexpr int EXTENDED_STATUS_GL_WHILE_EMISSION_LOW_BIT = 5;
static constexpr int EXTENDED_STATUS_MODE_CHANGE_DURING_EMISSION_BIT = 6; // TODO NIC: sentire longoni

static constexpr int EXTENDED_STATUS_INTERFACE_PS_HOT_STOP = 0x01 << EXTENDED_STATUS_INTERFACE_PS_HOT_STOP_BIT;
static constexpr int EXTENDED_STATUS_MAIN_PS_HOT_STOP = 0x01 << EXTENDED_STATUS_MAIN_PS_HOT_STOP_BIT;
static constexpr int EXTENDED_STATUS_OPTICAL_LINK_HOT_STOP = 0x01 << EXTENDED_STATUS_OPTICAL_LINK_HOT_STOP_BIT;
static constexpr int EXTENDED_STATUS_SAFETY_OPEN_WHILE_EMISSION_STOP = 0x01 << EXTENDED_STATUS_SAFETY_OPEN_WHILE_EMISSION_STOP_BIT;
static constexpr int EXTENDED_STATUS_EXTERNAL_STOP_WHILE_EMISSION = 0x01 << EXTENDED_STATUS_EXTERNAL_STOP_WHILE_EMISSION_BIT;
static constexpr int EXTENDED_STATUS_GL_WHILE_EMISSION_LOW = 0x01 << EXTENDED_STATUS_GL_WHILE_EMISSION_LOW_BIT;
static constexpr int EXTENDED_STATUS_MODE_CHANGE_DURING_EMISSION = 0x01 << EXTENDED_STATUS_MODE_CHANGE_DURING_EMISSION_BIT;

/***************************************************/

// bit mask flag configurazione laser
static constexpr unsigned int LASER_CONFIGURATION_OPTICAL_MODULE_2 = 0x00000001;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED1 = 0x00000002;
static constexpr unsigned int LASER_CONFIGURATION_REMOTE_UI = 0x00000004;
static constexpr unsigned int LASER_CONFIGURATION_GL = 0x00000008;
static constexpr unsigned int LASER_CONFIGURATION_PROCESS_FIBER_ALARM = 0x00000010;
static constexpr unsigned int LASER_CONFIGURATION_FEEDING_FIBER_ALARM = 0x00000020;
static constexpr unsigned int LASER_CONFIGURATION_FIELDBUS = 0x00000040;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED2 = 0x00000080;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED3 = 0x00000100;
static constexpr unsigned int LASER_CONFIGURATION_POWER_SUPPLY_CONTROL = 0x00000200;
static constexpr unsigned int LASER_CONFIGURATION_ANALOG_POWER_CONTROL = 0x00000400;
static constexpr unsigned int LASER_CONFIGURATION_SAFETY_CONTROL = 0x00000800;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED4 = 0x00001000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED5 = 0x00002000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED6 = 0x00004000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED7 = 0x00008000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED8 = 0x00010000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED9 = 0x00020000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED10 = 0x00040000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED11 = 0x00080000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED12 = 0x00100000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED13 = 0x00200000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED14 = 0x00400000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED15 = 0x00800000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED16 = 0x01000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED17 = 0x02000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED18 = 0x04000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED19 = 0x08000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED20 = 0x10000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED21 = 0x20000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED22 = 0x40000000;
static constexpr unsigned int LASER_CONFIGURATION_RESERVED23 = 0x80000000;

// bit mask flag opzioni laser
static constexpr unsigned int LASER_OPTIONS_HPB_INTERFACE = 0x00000001;
static constexpr unsigned int LASER_OPTIONS_RS232C_INTERFACE = 0x00000002;
static constexpr unsigned int LASER_OPTIONS_ALL_YLP_OLRC_INTERFACES = 0x00000004;
static constexpr unsigned int LASER_OPTIONS_RESERVED1 = 0x00000008;
static constexpr unsigned int LASER_OPTIONS_APD = 0x00000010;
static constexpr unsigned int LASER_OPTIONS_EXTENDED_PRR = 0x00000020;
static constexpr unsigned int LASER_OPTIONS_RESERVED2 = 0x00000040;
static constexpr unsigned int LASER_OPTIONS_RESERVED3 = 0x00000080;
static constexpr unsigned int LASER_OPTIONS_JUMP_PRR = 0x00000100;
static constexpr unsigned int LASER_OPTIONS_RESERVED4 = 0x00000200;
static constexpr unsigned int LASER_OPTIONS_BITSTREAM = 0x00000400;
static constexpr unsigned int LASER_OPTIONS_RESERVED5 = 0x00000800;
static constexpr unsigned int LASER_OPTIONS_RESERVED6 = 0x00001000;
static constexpr unsigned int LASER_OPTIONS_RESERVED7 = 0x00002000;
static constexpr unsigned int LASER_OPTIONS_RESERVED8 = 0x00004000;
static constexpr unsigned int LASER_OPTIONS_AUTO_LATCH_POWER = 0x00008000;
static constexpr unsigned int LASER_OPTIONS_GL_INSTALLED = 0x00010000;
static constexpr unsigned int LASER_OPTIONS_HIGH_CONTRAST = 0x00020000;
static constexpr unsigned int LASER_OPTIONS_RESERVED9 = 0x00040000;
static constexpr unsigned int LASER_OPTIONS_SWEEP_PRR = 0x00080000;
static constexpr unsigned int LASER_OPTIONS_RESERVED10 = 0x00100000;
static constexpr unsigned int LASER_OPTIONS_RESERVED11 = 0x00200000;
static constexpr unsigned int LASER_OPTIONS_RESERVED12 = 0x00400000;
static constexpr unsigned int LASER_OPTIONS_RESERVED13 = 0x00800000;
static constexpr unsigned int LASER_OPTIONS_RESERVED14 = 0x01000000;
static constexpr unsigned int LASER_OPTIONS_GL_SAFE = 0x02000000;
static constexpr unsigned int LASER_OPTIONS_DISCHARGE_SAFE = 0x04000000;
static constexpr unsigned int LASER_OPTIONS_RESERVED15 = 0x08000000;
static constexpr unsigned int LASER_OPTIONS_RESERVED16 = 0x10000000;
static constexpr unsigned int LASER_OPTIONS_RESERVED17 = 0x20000000;
static constexpr unsigned int LASER_OPTIONS_RESERVED18 = 0x40000000;
static constexpr unsigned int LASER_OPTIONS_RESERVED19 = 0x80000000;

// flag associato al campo network (GetNetworkSettingsMarshaller)
static constexpr int NETWORK_CONFIGURATION_MASK_DHCP_ENABLED = 0x01;
static constexpr int NETWORK_CONFIGURATION_MASK_FTP_ENABLED = 0x02;

// lista codici comandi
static constexpr int COMMAND_GET_LASERID = 1;
static constexpr int COMMAND_GET_NETWORK_SETTINGS = 2;
static constexpr int COMMAND_SET_IPSETTINGS = 3;
static constexpr int COMMAND_SET_COMMAND_TIMEOUT = 4;
static constexpr int COMMAND_GET_CRITICAL_ERROR_COUNTER = 5;
static constexpr int COMMAND_GET_TIMEDATA = 2002;
static constexpr int COMMAND_SET_CURRENT_TIME = 2003;
static constexpr int COMMAND_GET_FIELDBUS_STATIC_PARAMETRS = 2040;
static constexpr int COMMAND_GET_FIELDBUS_CONTROLLER_PARAMETRS = 2041;
static constexpr int COMMAND_GET_FIELDBUS_DATA = 2042;
static constexpr int COMMAND_SET_FIELDBUS_PARAMETRS = 2043;
static constexpr int COMMAND_SET_FIELDBUS_SLOT_PARAMETRS = 2044;
static constexpr int COMMAND_SET_FIELDBUS_SLOT_SPECIFIC_PARAMETERS = 2045;
static constexpr int COMMAND_RESET = 2506;
static constexpr int COMMAND_RESET_CRITICAL_ERROR = 2507;
static constexpr int COMMAND_GET_LASER_MODEL_INFO = 4001;
static constexpr int COMMAND_GET_LASER_SOFTWARE_INFO = 4002;
static constexpr int COMMAND_GET_MODE_PARAMETER = 4003;
static constexpr int COMMAND_GET_PARALLELIO_DATA = 4004;
static constexpr int COMMAND_GET_ALARM_COUNTER = 4005;
static constexpr int COMMAND_SET_LASER_INTERFACE = 4010;
static constexpr int COMMAND_GET_LASER_INTERFACE = 4011;
static constexpr int COMMAND_SET_MODE_INDEX = 4020;
static constexpr int COMMAND_SET_PS_MODE = 4021;
static constexpr int COMMAND_SET_GL = 4022;
static constexpr int COMMAND_SET_FREQUENCY = 4023;
static constexpr int COMMAND_SET_POWER = 4024;
static constexpr int COMMAND_SET_EE = 4025;
static constexpr int COMMAND_SET_EM = 4026;
static constexpr int COMMAND_GET_LASER_STATUS = 4030;
static constexpr int COMMAND_GET_FIRST_WORK_LOG = 4052;
static constexpr int COMMAND_GET_FIRST_ERROR_LOG = 4053;
static constexpr int COMMAND_WRITE_BINARY_BLOCK = 4601;
static constexpr int COMMAND_START_FILE_PROCESSING = 4602;
static constexpr int COMMAND_GET_LASER_STATE = 4603;
static constexpr int COMMAND_GET_FILE_INFO = 4612;
static constexpr int COMMAND_READ_BINARY_BLOCK = 4615;

// codici esito comando
static constexpr quint32 EXEC_OP_CODE_OK = 0;
static constexpr quint32 EXEC_OP_CODE_RESTR_LIMITS = 1;
static constexpr quint32 EXEC_OP_CODE_RESTR_DISCRETE = 2;
static constexpr quint32 EXEC_OP_CODE_LASER_BUSY = 3;
static constexpr quint32 EXEC_OP_CODE_UNKNOWN_COMMAND = 65535;
static constexpr quint32 EXEC_OP_CODE_UNSUPPORTED_COMMAND = 65534;
static constexpr quint32 EXEC_OP_CODE_OBSOLETE_COMMAND = 65533;
static constexpr quint32 EXEC_OP_CODE_OUT_OF_RANGE = 65532;
static constexpr quint32 EXEC_OP_CODE_INVALID_PARAMETER = 65531;
static constexpr quint32 EXEC_OP_CODE_ACCESS_LEVEL_RESTRICTION = 65530;
static constexpr quint32 EXEC_OP_CODE_CANNOT_EXECUTE_NOW = 65529;
static constexpr quint32 EXEC_OP_CODE_INVALUD_CHECKSUM = 65528;
static constexpr quint32 EXEC_OP_CODE_INTERNAL_ERROR = 65527;
static constexpr quint32 EXEC_OP_CODE_INVALID_DATA_SIZE = 65526;

}

#endif // IPG_CONSTANTS_HPP
