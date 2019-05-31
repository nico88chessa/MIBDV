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
            case IOType::DOOR: return tr(UTILS_IOTYPE_DOOR);
            case IOType::BYPASS_SECURITY: return tr(UTILS_IOTYPE_BYPASS_SECURITY);
            case IOType::WATER: return tr(UTILS_IOTYPE_WATER);
            case IOType::MARK_IN_PROGRESS: return tr(UTILS_IOTYPE_MARK_IN_PROGRESS);
            case IOType::SCANNER_READY: return tr(UTILS_IOTYPE_SCANNER_READY);
            case IOType::SCANNER_ERROR: return tr(UTILS_IOTYPE_SCANNER_ERROR);
            case IOType::GENERIC_INPUT: return tr(UTILS_IOTYPE_GENERIC_INPUT);
            case IOType::LASER_POWER: return tr(UTILS_IOTYPE_LASER_POWER);
            case IOType::COMPRESSED_AIR_1: return tr(UTILS_IOTYPE_COMPRESSED_AIR_1);
            case IOType::COMPRESSED_AIR_2: return tr(UTILS_IOTYPE_COMPRESSED_AIR_2);
            case IOType::BRUSH_1: return tr(UTILS_IOTYPE_BRUSH_1);
            case IOType::BRUSH_2: return tr(UTILS_IOTYPE_BRUSH_2);
            case IOType::SUCTION: return tr(UTILS_IOTYPE_SUCTION);
            case IOType::ENABLE_AIR_FOR_FLUID: return tr(UTILS_IOTYPE_ENABLE_AIR_FOR_FLUID);
            case IOType::ENABLE_FLUID: return tr(UTILS_IOTYPE_ENABLE_FLUID);
            case IOType::POWER_SCAN: return tr(UTILS_IOTYPE_POWER_SCAN);
            case IOType::START_SCAN: return tr(UTILS_IOTYPE_START_SCAN);
            case IOType::STOP_SCAN: return tr(UTILS_IOTYPE_STOP_SCAN);
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
            case IOType::DOOR: return "DOOR";
            case IOType::BYPASS_SECURITY: return "BYPASS_SECURITY";
            case IOType::WATER: return "WATER";
            case IOType::MARK_IN_PROGRESS: return "MARK_IN_PROGRESS";
            case IOType::SCANNER_READY: return "SCANNER_READY";
            case IOType::SCANNER_ERROR: return "SCANNER_ERROR";
            case IOType::GENERIC_INPUT: return "GENERIC_INPUT";
            case IOType::LASER_POWER: return "LASER_POWER";
            case IOType::COMPRESSED_AIR_1: return "COMPRESSED_AIR_1";
            case IOType::COMPRESSED_AIR_2: return "COMPRESSED_AIR_2";
            case IOType::BRUSH_1: return "BRUSH_1";
            case IOType::BRUSH_2: return "BRUSH_2";
            case IOType::SUCTION: return "SUCTION";
            case IOType::ENABLE_AIR_FOR_FLUID: return "ENABLE_AIR_FOR_FLUID";
            case IOType::ENABLE_FLUID: return "ENABLE_FLUID";
            case IOType::POWER_SCAN: return "POWER_SCAN";
            case IOType::START_SCAN: return "START_SCAN";
            case IOType::STOP_SCAN: return "STOP_SCAN";
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
        if (str.compare("DOOR", Qt::CaseInsensitive)==0)
            return IOType::DOOR;
        if (str.compare("BYPASS_SECURITY", Qt::CaseInsensitive)==0)
            return IOType::BYPASS_SECURITY;
        if (str.compare("WATER", Qt::CaseInsensitive)==0)
            return IOType::WATER;
        if (str.compare("MARK_IN_PROGRESS", Qt::CaseInsensitive)==0)
            return IOType::MARK_IN_PROGRESS;
        if (str.compare("SCANNER_READY", Qt::CaseInsensitive)==0)
            return IOType::SCANNER_READY;
        if (str.compare("SCANNER_ERROR", Qt::CaseInsensitive)==0)
            return IOType::SCANNER_ERROR;
        if (str.compare("GENERIC_INPUT", Qt::CaseInsensitive)==0)
            return IOType::GENERIC_INPUT;
        if (str.compare("LASER_POWER", Qt::CaseInsensitive)==0)
            return IOType::LASER_POWER;
        if (str.compare("COMPRESSED_AIR_1", Qt::CaseInsensitive)==0)
            return IOType::COMPRESSED_AIR_1;
        if (str.compare("COMPRESSED_AIR_2", Qt::CaseInsensitive)==0)
            return IOType::COMPRESSED_AIR_2;
        if (str.compare("BRUSH_1", Qt::CaseInsensitive)==0)
            return IOType::BRUSH_1;
        if (str.compare("BRUSH_2", Qt::CaseInsensitive)==0)
            return IOType::BRUSH_2;
        if (str.compare("SUCTION", Qt::CaseInsensitive)==0)
            return IOType::SUCTION;
        if (str.compare("ENABLE_AIR_FOR_FLUID", Qt::CaseInsensitive)==0)
            return IOType::ENABLE_AIR_FOR_FLUID;
        if (str.compare("ENABLE_FLUID", Qt::CaseInsensitive)==0)
            return IOType::ENABLE_FLUID;
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
            case DeviceKey::MOTION_SIGNALER: return "MOTION_SIGNALER";
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
        if (str.compare("MOTION_SIGNALER", Qt::CaseInsensitive)==0)
            return DeviceKey::MOTION_SIGNALER;

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

};

}

#endif // UTILS_HPP
