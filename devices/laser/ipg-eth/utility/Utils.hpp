#ifndef IPG_UTILS_HPP
#define IPG_UTILS_HPP

#include <QString>

#include "Constants.hpp"

namespace ipg {

class Utils {

public:
    static QString getAlarmDescription(const int alarmCode) {

        switch (alarmCode) {
        case ALARM_SYSTEM_ALARM: return "System alarm";
        case ALARM_CRITICAL_ALARM: return "Critical alarm";
        case ALARM_TEMPERATURE_ALARM: return "Temperature alarm";
        case ALARM_INTERFACE_CONTROLLER_24V_ALARM: return "Interface controller 24V alarm OM1";
        case ALARM_BACK_REFLECTION_ALARM: return "Back reflection alarm";
        case ALARM_OPTICAL_LINK_ALARM: return "Optical link alarm";
        case ALARM_PROCESS_FIBER_ALARM: return "Process fiber alarm";
        default: return "";
        }
    }

    static QString getSystemAlarmDescription(const int systemAlarmCode) {

        switch (systemAlarmCode) {
        case SYSTEM_ALARM_LASER_INTERFACE: return "Laser Interface";
        case SYSTEM_ALARM_OPTICAL_MODULE_1: return "Optical module 1";
        case SYSTEM_ALARM_OPTICAL_MODULE_2: return "Optical module 2";
        case SYSTEM_ALARM_POWER_SUPPLY: return "Power Supply";
        case SYSTEM_ALARM_SAFETY: return "Safety";
        case SYSTEM_ALARM_COMMUNICATION_OM1: return "Communication OM1";
        case SYSTEM_ALARM_COMMUNICATION_OM2: return "Communication OM2";
        case SYSTEM_ALARM_COMMUNICATION_PS: return "Communication PS";
        case SYSTEM_ALARM_COMMUNICATION_SAFETY: return "Communication Safety";
        case SYSTEM_ALARM_FEEDING_FIBER_ALARM: return "Feeding fiber alarm";
        default: return "";

        }
    }

    static QString getExtendedAlarmDescription(const int extendedAlarmCode) {

        switch (extendedAlarmCode) {
        case EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_LOW: return "Interface voltage too low";
        case EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_HIGH: return "Interface voltage too high";
        case EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_LOW: return "Interface temperature too low";
        case EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_HIGH: return "Interface temperature too high";
        case EXTENDED_ALARM_LASER_TEMPERATURE_TOO_LOW: return "Laser temperature too low";
        case EXTENDED_ALARM_LASER_TEMPERATURE_TOO_HIGH: return "Laser temperature too high";
        case EXTENDED_ALARM_PS_TEMPERATURE_TOO_HIGH: return "PS temperature too high";
        default: return "";

        }

    }

    static QString getStatusDescription(const int statusCode) {

        switch (statusCode) {
        case STATUS_SAFETY_CIRCUIT_OPEN : return "Safety circuit open";
        case STATUS_POWER_SUPPLY_IS_ON : return "Power Supply is ON";
        case STATUS_LASER_READY : return "Laser Ready";
        case STATUS_EMISSION_IS_ON : return "Emission is ON";
        case STATUS_GUIDE_LASER : return "Guide laser";
        case STATUS_EXTERNAL_STOP : return "External Stop";
        case STATUS_SAFETY_CIRCUIT_BLOCKED : return "Safety circuit blocked";
        case STATUS_HOT_STOP : return "Hot Stop";
        default: return "";

        }

    }

    static QString getExtendedStatusDescription(const int extendedStatusCode) {

        switch (extendedStatusCode) {
        case EXTENDED_STATUS_INTERFACE_PS_HOT_STOP: return "Interface PS hot stop";
        case EXTENDED_STATUS_MAIN_PS_HOT_STOP: return "Main PS hot stop";
        case EXTENDED_STATUS_OPTICAL_LINK_HOT_STOP: return "Optical link hot stop";
        case EXTENDED_STATUS_SAFETY_OPEN_WHILE_EMISSION_STOP: return "Safety open while emission stop";
        case EXTENDED_STATUS_EXTERNAL_STOP_WHILE_EMISSION: return "External Stop while emission";
        case EXTENDED_STATUS_GL_WHILE_EMISSION_LOW: return "GL while emission low";
        case EXTENDED_STATUS_MODE_CHANGE_DURING_EMISSION: return "Mode change during emission";
        default: return "";

        }

    }

    static QString getWarningDescription(const int warningCode) {

        switch(warningCode) {
        case WARNINGS_PRR_LOW: return "PRR low";
        case WARNINGS_PRR_HIGH: return "PRR high";
        case WARNINGS_SDCARD_FAILURE: return "SDcard failure";
        case WARNINGS_INTERFACE_TEMPERATURE_TOO_LOW: return "Interface temperature too low";
        case WARNINGS_INTERFACE_TEMPERATURE_TOO_HIGH: return "Interface temperature too high";
        case WARNINGS_LASER_TEMPERATURE_TOO_LOW: return "Laser temperature too low";
        case WARNINGS_LASER_TEMPERATURE_TOO_HIGH: return "Laser temperature too high";
        default: return "";
        }

    }

    static QString getExecOpCodeDescription(const quint32 value) {

        switch (value) {
        case EXEC_OP_CODE_OK: return "OK - The command is successful";
        case EXEC_OP_CODE_RESTR_LIMITS: return "Command was executed with restriction by limits";
        case EXEC_OP_CODE_RESTR_DISCRETE: return "Command was executed with restriction by discrete (step)";
        case EXEC_OP_CODE_LASER_BUSY: return "The laser is busy (the laser is doing a long-running operation). The current status of long-running operations can be checked by the command GetLaserState.";
        case EXEC_OP_CODE_UNKNOWN_COMMAND: return "Unknown command";
        case EXEC_OP_CODE_UNSUPPORTED_COMMAND: return "Unsupported command (returned if the command has no meaning regarding to the current laser configuration)";
        case EXEC_OP_CODE_OBSOLETE_COMMAND: return "Obsolete command (returned if the command is outdated and not supported by laser firmware anymore)";
        case EXEC_OP_CODE_OUT_OF_RANGE: return "Command was not executed, because parameter is out of range";
        case EXEC_OP_CODE_INVALID_PARAMETER: return "Command was not executed, because has invalid parameter";
        case EXEC_OP_CODE_ACCESS_LEVEL_RESTRICTION: return "Command was not executed by access level restriction";
        case EXEC_OP_CODE_CANNOT_EXECUTE_NOW: return "Command can not be executed now (at present time)";
        case EXEC_OP_CODE_INVALUD_CHECKSUM: return "Invalid checksum CRC16";
        case EXEC_OP_CODE_INTERNAL_ERROR: return "Internal Error";
        case EXEC_OP_CODE_INVALID_DATA_SIZE: return "Invalid data size";
        default: return "";
        }

    }

    static inline QStringList getLaserInstalledOptionsString(IPG_ULONG options) {

        QStringList list;
        if (options & LASER_OPTIONS_HPB_INTERFACE) list.append("HPB Interface");
        if (options & LASER_OPTIONS_RS232C_INTERFACE) list.append("RS232C Interface");
        if (options & LASER_OPTIONS_ALL_YLP_OLRC_INTERFACES) list.append("ALL-YLP-OLRC Interfaces");
        if (options & LASER_OPTIONS_APD) list.append("APD");
        if (options & LASER_OPTIONS_EXTENDED_PRR) list.append("Extended PRR");
        if (options & LASER_OPTIONS_JUMP_PRR) list.append("Jump PRR");
        if (options & LASER_OPTIONS_BITSTREAM) list.append("Bitstream");
        if (options & LASER_OPTIONS_AUTO_LATCH_POWER) list.append("Auto Latch Power");
        if (options & LASER_OPTIONS_GL_INSTALLED) list.append("GL Installed");
        if (options & LASER_OPTIONS_HIGH_CONTRAST) list.append("High Contrast");
        if (options & LASER_OPTIONS_SWEEP_PRR) list.append("Sweep PRR");
        if (options & LASER_OPTIONS_GL_SAFE) list.append("GL Safe");
        if (options & LASER_OPTIONS_DISCHARGE_SAFE) list.append("Discharge Safe");

        return list;

    }


    static inline QStringList getLaserConfigurationString(IPG_ULONG configuration) {

        QStringList list;
        if (configuration & LASER_CONFIGURATION_OPTICAL_MODULE_2) list.append("Optical Module 2");
        if (configuration & LASER_CONFIGURATION_REMOTE_UI) list.append("Remote Ui");
        if (configuration & LASER_CONFIGURATION_GL) list.append("GL");
        if (configuration & LASER_CONFIGURATION_PROCESS_FIBER_ALARM) list.append("Process Fiber Alarm");
        if (configuration & LASER_CONFIGURATION_FEEDING_FIBER_ALARM) list.append("Feeding Fiber Alarm");
        if (configuration & LASER_CONFIGURATION_FIELDBUS) list.append("Fieldbus");
        if (configuration & LASER_CONFIGURATION_POWER_SUPPLY_CONTROL) list.append("Power Supply Control");
        if (configuration & LASER_CONFIGURATION_ANALOG_POWER_CONTROL) list.append("Analog Power Control");
        if (configuration & LASER_CONFIGURATION_SAFETY_CONTROL) list.append("Safety Control");

        return list;

    }

    static inline IPG_USHORT getCrc16(const unsigned char* data, int dataSize) {

        unsigned short crc16 = 0;

        for (int n=0; n<dataSize; n++) {

            unsigned short ch = static_cast<unsigned short>(data[n]);
            for (int i=0; i<8; i++) {
                unsigned short b = (crc16 ^ ch) & 1;
                crc16 >>= 1;
                ch >>= 1;
                if (b)
                    crc16 ^= 0xA001;

            }

        }

        return crc16;

    }

    static inline QString getStringFromMacAddress(QByteArray macAddress) {
        return QString("%1:%2:%3:%4:%5:%6").
                arg(static_cast<unsigned char>(macAddress.at(0)), 2, 16, QLatin1Char('0')).
                arg(static_cast<unsigned char>(macAddress.at(1)), 2, 16, QLatin1Char('0')).
                arg(static_cast<unsigned char>(macAddress.at(2)), 2, 16, QLatin1Char('0')).
                arg(static_cast<unsigned char>(macAddress.at(3)), 2, 16, QLatin1Char('0')).
                arg(static_cast<unsigned char>(macAddress.at(4)), 2, 16, QLatin1Char('0')).
                arg(static_cast<unsigned char>(macAddress.at(5)), 2, 16, QLatin1Char('0')).toUpper();
    }


    static inline int getMainInterface(const QString& name) {

        if (name.compare(INTERFACE_CONFIG_STRING_LIST[0], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_RS232;
        else if (name.compare(INTERFACE_CONFIG_STRING_LIST[1], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_ETHERNET;
        else if (name.compare(INTERFACE_CONFIG_STRING_LIST[2], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_PARALLEL_IO;
        else if (name.compare(INTERFACE_CONFIG_STRING_LIST[3], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_FIELDBUS;
        else if (name.compare(INTERFACE_CONFIG_STRING_LIST[4], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_HW;
        else if (name.compare(INTERFACE_CONFIG_STRING_LIST[5], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_AUXILIARY;
        else if (name.compare(INTERFACE_CONFIG_STRING_LIST[6], Qt::CaseInsensitive) == 0)
            return INTERFACE_CONFIG_MAIN;

        return -1;

    }

    static inline QString getMainInterface(int index) {

        switch(index) {
        case MAIN_INTERFACE_RS232: return MAIN_INTERFACE_STRING_LIST[0];
        case MAIN_INTERFACE_ETHERNET: return MAIN_INTERFACE_STRING_LIST[1];
        case MAIN_INTERFACE_PARALLEL_IO: return MAIN_INTERFACE_STRING_LIST[2];
        case MAIN_INTERFACE_FIELDBUS: return MAIN_INTERFACE_STRING_LIST[3];
        }

        return "";

    }

    static inline int getInterfaceConfig(const QString& name) {

        if (name.compare(MAIN_INTERFACE_STRING_LIST[0], Qt::CaseInsensitive) == 0)
            return MAIN_INTERFACE_RS232;
        if (name.compare(MAIN_INTERFACE_STRING_LIST[1], Qt::CaseInsensitive) == 0)
            return MAIN_INTERFACE_ETHERNET;
        if (name.compare(MAIN_INTERFACE_STRING_LIST[2], Qt::CaseInsensitive) == 0)
            return MAIN_INTERFACE_PARALLEL_IO;
        if (name.compare(MAIN_INTERFACE_STRING_LIST[3], Qt::CaseInsensitive) == 0)
            return MAIN_INTERFACE_FIELDBUS;

        return -1;

    }

    static inline QString getInterfaceConfig(int index) {
        switch (index) {
        case INTERFACE_CONFIG_RS232: return INTERFACE_CONFIG_STRING_LIST[0];
        case INTERFACE_CONFIG_ETHERNET: return INTERFACE_CONFIG_STRING_LIST[1];
        case INTERFACE_CONFIG_PARALLEL_IO: return INTERFACE_CONFIG_STRING_LIST[2];
        case INTERFACE_CONFIG_FIELDBUS: return INTERFACE_CONFIG_STRING_LIST[3];
        case INTERFACE_CONFIG_HW: return INTERFACE_CONFIG_STRING_LIST[4];
        case INTERFACE_CONFIG_AUXILIARY: return INTERFACE_CONFIG_STRING_LIST[5];
        case INTERFACE_CONFIG_MAIN: return INTERFACE_CONFIG_STRING_LIST[6];
        }

        return "";

    }

};

}

#endif // IPG_UTILS_HPP
