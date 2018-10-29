#ifndef UTILS_HPP
#define UTILS_HPP

#include <Constants.hpp>
#include <QString>

namespace PROGRAM_NAMESPACE {

class Utils {

public:

    static QString getIODescription(IOType type) {

        switch (type) {
            case IOType::NOT_VALID: break;
            case IOType::POWER: return QT_TR_NOOP("Power");
            case IOType::CYCLE: return QT_TR_NOOP("Cycle");
            case IOType::EMERGENCY_MUSHROOM: return QT_TR_NOOP("Emergency Mushroom");
            case IOType::DOOR: return QT_TR_NOOP("Door");
            case IOType::BYPASS_SECURITY: return QT_TR_NOOP("Bypass Security");
            case IOType::WATER: return QT_TR_NOOP("Water");
            case IOType::MARK_IN_PROGRESS: return QT_TR_NOOP("Mark In Progress");
            case IOType::SCANNER_READY: return QT_TR_NOOP("Scanner Ready");
            case IOType::SCANNER_ERROR: return QT_TR_NOOP("Scanner Error");
            case IOType::LASER_POWER: return QT_TR_NOOP("Laser Power");
            case IOType::COMPRESSED_AIR_1: return QT_TR_NOOP("Compressed Air 1");
            case IOType::COMPRESSED_AIR_2: return QT_TR_NOOP("Compressed Air 2");
            case IOType::BRUSH_1: return QT_TR_NOOP("Brush 1");
            case IOType::BRUSH_2: return QT_TR_NOOP("Brush 2");
            case IOType::SUCTION: return QT_TR_NOOP("Suction");
            case IOType::ENABLE_AIR_FOR_FLUID: return QT_TR_NOOP("Enable Air For Fluid");
            case IOType::ENABLE_FLUID: return QT_TR_NOOP("Enable Fluid");
            case IOType::POWER_SCAN: return QT_TR_NOOP("Power Scan");
            case IOType::START_SCAN: return QT_TR_NOOP("Start Scan");
            case IOType::STOP_SCAN: return QT_TR_NOOP("Stop Scan");
            case IOType::RED_LIGHT: return QT_TR_NOOP("Red Light");
            case IOType::GREEN_LIGHT: return QT_TR_NOOP("Green Light");
            case IOType::ANAL_AUTOFOCUS_DISTANCE: return QT_TR_NOOP("Autofocus Distance");
            case IOType::ANAL_SUCTION_DEPRESSURE: return QT_TR_NOOP("Suction Depressure");
            case IOType::ANAL_AIR_PRESSURE: return QT_TR_NOOP("Air Pressure");
        }

        return "";
    }

    static QString getStringFromIOType(IOType type) {

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
            case IOType::ANAL_AUTOFOCUS_DISTANCE: return "ANAL_AUTOFOCUS_DISTANCE";
            case IOType::ANAL_SUCTION_DEPRESSURE: return "ANAL_SUCTION_DEPRESSURE";
            case IOType::ANAL_AIR_PRESSURE: return "ANAL_AIR_PRESSURE";
        }

        return "";
    }

    static IOType getIOTypeFromString(const QString& str) {

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
        if (str.compare("ANAL_AUTOFOCUS_DISTANCE", Qt::CaseInsensitive)==0)
            return IOType::ANAL_AUTOFOCUS_DISTANCE;
        if (str.compare("ANAL_SUCTION_DEPRESSURE", Qt::CaseInsensitive)==0)
            return IOType::ANAL_SUCTION_DEPRESSURE;
        if (str.compare("ANAL_AIR_PRESSUR", Qt::CaseInsensitive)==0)
            return IOType::ANAL_AIR_PRESSURE;
        return IOType::NOT_VALID;

    }

    static QString getDeviceKeyStr(DeviceKey device) {

        switch (device) {
            case DeviceKey::NONE: break;
            case DeviceKey::GALIL_CN: return "GALIL_CN";
            case DeviceKey::GALIL_PLC: return "GALIL_PLC";
        }

        return "";
    }

};

}

#endif // UTILS_HPP
