#ifndef UTILS_HPP
#define UTILS_HPP

#include <QtCore>
#include <QString>

#include <Constants.hpp>
#include <gui/resources/lang/lang.hpp>


namespace PROGRAM_NAMESPACE {

class Utils {
    Q_DECLARE_TR_FUNCTIONS(Utils)

public:

    static inline QString getIODescription(IOType type) {

        switch (type) {
            case IOType::NOT_VALID: break;
            case IOType::POWER: return tr(UTILS_IOTYPE_POWER);
            case IOType::CYCLE: return tr(UTILS_IOTYPE_CYCLE);
            case IOType::EMERGENCY_MUSHROOM: return tr(UTILS_IOTYPE_EMERGENCY_MUSHROOM);
            case IOType::DOOR_OPEN: return tr(UTILS_IOTYPE_DOOR_OPEN);
            case IOType::MAINTENANCE: return tr(UTILS_IOTYPE_MAINTENANCE);
            case IOType::WATER_ALARM: return tr(UTILS_IOTYPE_WATER_ALARM);
            case IOType::MARK_IN_PROGRESS: return tr(UTILS_IOTYPE_MARK_IN_PROGRESS);
            case IOType::SCANNER_READY: return tr(UTILS_IOTYPE_SCANNER_READY);
            case IOType::SCANNER_ERROR: return tr(UTILS_IOTYPE_SCANNER_ERROR);
            case IOType::DISTANCE_SENSOR_FAULT: return tr(UTILS_IOTYPE_DISTANCE_SENSOR_FAULT);
            case IOType::DEPRESSURE_SENSOR_FAULT: return tr(UTILS_IOTYPE_DEPRESSURE_SENSOR_FAULT);
            case IOType::PRESSURE_1_SENSOR_FAULT: return tr(UTILS_IOTYPE_PRESSURE_1_SENSOR_FAULT);
            case IOType::PRESSURE_2_SENSOR_FAULT: return tr(UTILS_IOTYPE_PRESSURE_2_SENSOR_FAULT);
            case IOType::GENERIC_INPUT: return tr(UTILS_IOTYPE_GENERIC_INPUT);
            case IOType::LASER_POWER: return tr(UTILS_IOTYPE_LASER_POWER);
            case IOType::COMPRESSED_AIR_1: return tr(UTILS_IOTYPE_COMPRESSED_AIR_1);
            case IOType::COMPRESSED_AIR_2: return tr(UTILS_IOTYPE_COMPRESSED_AIR_2);
            case IOType::SUCTION: return tr(UTILS_IOTYPE_SUCTION);
            case IOType::POWER_SCAN: return tr(UTILS_IOTYPE_POWER_SCAN);
            case IOType::START_SCAN: return tr(UTILS_IOTYPE_START_SCAN);
            case IOType::STOP_SCAN: return tr(UTILS_IOTYPE_STOP_SCAN);
            case IOType::YELLOW_LIGHT: return tr(UTILS_IOTYPE_YELLOW_LIGHT);
            case IOType::RED_LIGHT: return tr(UTILS_IOTYPE_RED_LIGHT);
            case IOType::GREEN_LIGHT: return tr(UTILS_IOTYPE_GREEN_LIGHT);
            case IOType::GENERIC_ANALOG_INPUT: return tr(UTILS_IOTYPE_GENERIC_ANALOG_INPUT);
        }

        return "";

    }

    static inline QString getStringFromIOType(IOType type) {

        switch (type) {
            case IOType::NOT_VALID: break;
            case IOType::POWER: return "POWER";
            case IOType::CYCLE: return "CYCLE";
            case IOType::EMERGENCY_MUSHROOM: return "EMERGENCY_MUSHROOM";
            case IOType::DOOR_OPEN: return "DOOR_OPEN";
            case IOType::MAINTENANCE: return "MAINTENANCE";
            case IOType::WATER_ALARM: return "WATER_ALARM";
            case IOType::MARK_IN_PROGRESS: return "MARK_IN_PROGRESS";
            case IOType::SCANNER_READY: return "SCANNER_READY";
            case IOType::SCANNER_ERROR: return "SCANNER_ERROR";
            case IOType::DISTANCE_SENSOR_FAULT: return "DISTANCE_SENSOR_FAULT";
            case IOType::DEPRESSURE_SENSOR_FAULT: return "DEPRESSURE_SENSOR_FAULT";
            case IOType::PRESSURE_1_SENSOR_FAULT: return "PRESSSURE_1_SENSOR_FAULT";
            case IOType::PRESSURE_2_SENSOR_FAULT: return "PRESSSURE_2_SENSOR_FAULT";
            case IOType::GENERIC_INPUT: return "GENERIC_INPUT";
            case IOType::LASER_POWER: return "LASER_POWER";
            case IOType::COMPRESSED_AIR_1: return "COMPRESSED_AIR_1";
            case IOType::COMPRESSED_AIR_2: return "COMPRESSED_AIR_2";
            case IOType::SUCTION: return "SUCTION";
            case IOType::POWER_SCAN: return "POWER_SCAN";
            case IOType::START_SCAN: return "START_SCAN";
            case IOType::STOP_SCAN: return "STOP_SCAN";
            case IOType::YELLOW_LIGHT: return "YELLOW_LIGHT";
            case IOType::RED_LIGHT: return "RED_LIGHT";
            case IOType::GREEN_LIGHT: return "GREEN_LIGHT";
            case IOType::GENERIC_ANALOG_INPUT: return "GENERIC_ANALOG_INPUT";
        }

        return "";

    }

    static inline IOType getIOTypeFromString(const QString& str) {

        if (str.compare("POWER", Qt::CaseInsensitive)==0)
            return IOType::POWER;
        if (str.compare("CYCLE", Qt::CaseInsensitive)==0)
            return IOType::CYCLE;
        if (str.compare("EMERGENCY_MUSHROOM", Qt::CaseInsensitive)==0)
            return IOType::EMERGENCY_MUSHROOM;
        if (str.compare("DOOR_OPEN", Qt::CaseInsensitive)==0)
            return IOType::DOOR_OPEN;
        if (str.compare("MAINTENANCE", Qt::CaseInsensitive)==0)
            return IOType::MAINTENANCE;
        if (str.compare("WATER_ALARM", Qt::CaseInsensitive)==0)
            return IOType::WATER_ALARM;
        if (str.compare("MARK_IN_PROGRESS", Qt::CaseInsensitive)==0)
            return IOType::MARK_IN_PROGRESS;
        if (str.compare("SCANNER_READY", Qt::CaseInsensitive)==0)
            return IOType::SCANNER_READY;
        if (str.compare("SCANNER_ERROR", Qt::CaseInsensitive)==0)
            return IOType::SCANNER_ERROR;
        if (str.compare("DISTANCE_SENSOR_FAULT", Qt::CaseInsensitive)==0)
            return IOType::DISTANCE_SENSOR_FAULT;
        if (str.compare("DEPRESSURE_SENSOR_FAULT", Qt::CaseInsensitive)==0)
            return IOType::DEPRESSURE_SENSOR_FAULT;
        if (str.compare("PRESSSURE_1_SENSOR_FAULT", Qt::CaseInsensitive)==0)
            return IOType::PRESSURE_1_SENSOR_FAULT;
        if (str.compare("PRESSSURE_2_SENSOR_FAULT", Qt::CaseInsensitive)==0)
            return IOType::PRESSURE_2_SENSOR_FAULT;
        if (str.compare("GENERIC_INPUT", Qt::CaseInsensitive)==0)
            return IOType::GENERIC_INPUT;
        if (str.compare("LASER_POWER", Qt::CaseInsensitive)==0)
            return IOType::LASER_POWER;
        if (str.compare("COMPRESSED_AIR_1", Qt::CaseInsensitive)==0)
            return IOType::COMPRESSED_AIR_1;
        if (str.compare("COMPRESSED_AIR_2", Qt::CaseInsensitive)==0)
            return IOType::COMPRESSED_AIR_2;
        if (str.compare("SUCTION", Qt::CaseInsensitive)==0)
            return IOType::SUCTION;
        if (str.compare("POWER_SCAN", Qt::CaseInsensitive)==0)
            return IOType::POWER_SCAN;
        if (str.compare("START_SCAN", Qt::CaseInsensitive)==0)
            return IOType::START_SCAN;
        if (str.compare("STOP_SCAN", Qt::CaseInsensitive)==0)
            return IOType::STOP_SCAN;
        if (str.compare("RED_LIGHT", Qt::CaseInsensitive)==0)
            return IOType::RED_LIGHT;
        if (str.compare("GREEN_LIGHT", Qt::CaseInsensitive)==0)
            return IOType::GREEN_LIGHT;
        if (str.compare("GENERIC_ANALOG_INPUT", Qt::CaseInsensitive)==0)
            return IOType::GENERIC_ANALOG_INPUT;
        return IOType::NOT_VALID;

    }

    static inline QString getStringFromDeviceKey(DeviceKey device) {

        switch (device) {
            case DeviceKey::NONE: break;
            case DeviceKey::GALIL_CN: return "GALIL_CN";
            case DeviceKey::GALIL_CN_INSPECTOR: return "GALIL_CN_INSPECTOR";
            case DeviceKey::GALIL_PLC: return "GALIL_PLC";
            case DeviceKey::GALIL_PLC_INSPECTOR: return "GALIL_PLC_INSPECTOR";
            case DeviceKey::IO_SIGNALER: return "IO_SIGNALER";
            case DeviceKey::MOTION_ANALIZER: return "MOTION_ANALIZER";
            case DeviceKey::DEVICE_CONNECTION_WATCHER: return "DEV_CONN_WATCHER";
        }

        return "NONE";

    }

    static inline DeviceKey getDeviceKeyFromString(const QString& str) {

        if (str.compare("GALIL_CN", Qt::CaseInsensitive)==0)
            return DeviceKey::GALIL_CN;
        if (str.compare("GALIL_CN_INSPECTOR", Qt::CaseInsensitive)==0)
            return DeviceKey::GALIL_CN_INSPECTOR;
        if (str.compare("GALIL_PLC", Qt::CaseInsensitive)==0)
            return DeviceKey::GALIL_PLC;
        if (str.compare("GALIL_PLC_INSPECTOR", Qt::CaseInsensitive)==0)
            return DeviceKey::GALIL_PLC_INSPECTOR;
        if (str.compare("IO_SIGNALER", Qt::CaseInsensitive)==0)
            return DeviceKey::IO_SIGNALER;
        if (str.compare("MOTION_ANALIZER", Qt::CaseInsensitive)==0)
            return DeviceKey::MOTION_ANALIZER;
        if (str.compare("DEV_CONN_WATCHER", Qt::CaseInsensitive)==0)
            return DeviceKey::DEVICE_CONNECTION_WATCHER;

        return DeviceKey::NONE;

    }

    static inline QString getMotionStopCodeDescription(const MotionStopCode& value) {

        QString descr = "";

        switch(value) {
            case MotionStopCode::MOTION_NAN: descr = tr("Motion unknown stop code"); break;
            case MotionStopCode::MOTION_IS_RUNNING: descr = tr("Motion is running"); break;
            case MotionStopCode::MOTION_STOP_CORRECTLY: descr = tr("Motion stop correctly"); break;
            case MotionStopCode::MOTION_STOP_COMMAND: descr = tr("Motion stop command"); break;
            case MotionStopCode::MOTION_STOP_ON_ERROR: descr = tr("Motion stop on error"); break;
        }
        return descr;

    }

    static inline QString getMachineStatusDescription(const MachineStatus& status) {

        QString descr = "";
        switch (status) {
        case MachineStatus::IDLE: descr = tr("Idle"); break;
        case MachineStatus::STOP_RESUMABLE: descr = tr("Stop resumable"); break;
        case MachineStatus::PAUSE: descr = tr("Pause"); break;
        case MachineStatus::PRINTING: descr = tr("Printing"); break;
        }
        return descr;

    }

    static constexpr inline int makeErrorHumanReadable(const int value) {
        return value & ~PROGRAM_ERR_START_CODE;
    }

};

}

#endif // UTILS_HPP
