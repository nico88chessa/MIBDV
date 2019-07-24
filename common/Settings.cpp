#include "Settings.hpp"

using namespace PROGRAM_NAMESPACE;

#include <Logger.hpp>
#include <Utils.hpp>

const char* Settings::ARRAY_DIGITAL_INPUT = "DigitalInput";
const char* Settings::ARRAY_DIGITAL_OUTPUT = "DigitalOutput";
const char* Settings::ARRAY_ANALOG_INPUT = "AnalogInput";

Settings::Settings() {

    traceEnter;

    this->loadValuesFromFile();

    bool value = this->validateSettings();

    this->writeValuesToFile();

    traceExit;

}

void Settings::loadValuesFromFile() {

    traceEnter;

    using namespace settings;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION, APPLICATION_NAME);

    axisXStepPerMm = settings.value(AXIS_X_STEP_PER_MM, AXIS_X_STEP_PER_MM_DFLT).value<int>();
    axisXMinPosMm = settings.value(AXIS_X_MIN_POS_MM, AXIS_X_MIN_POS_MM_DFLT).value<int>();
    axisXMaxPosMm = settings.value(AXIS_X_MAX_POS_MM, AXIS_X_MAX_POS_MM_DFLT).value<int>();
    axisXBaseOffsetMm = settings.value(AXIS_X_BASE_OFFSET_MM, AXIS_X_BASE_OFFSET_MM_DFLT).value<real>();
    axisXManualSpeedMms = settings.value(AXIS_X_MANUAL_SPEED_MMS, AXIS_X_MANUAL_SPEED_MMS_DFLT).value<real>();
    axisXManualAccMms2 = settings.value(AXIS_X_MANUAL_ACC_MMS2, AXIS_X_MANUAL_ACC_MMS2_DFLT).value<real>();
    axisXManualDecMms2 = settings.value(AXIS_X_MANUAL_DEC_MMS2, AXIS_X_MANUAL_DEC_MMS2_DFLT).value<real>();
    axisXOperativeSpeedMms = settings.value(AXIS_X_OPERATIVE_SPEED_MMS, AXIS_X_OPERATIVE_SPEED_MMS_DFLT).value<real>();
    axisXOperativeAccMms2 = settings.value(AXIS_X_OPERATIVE_ACC_MMS2, AXIS_X_OPERATIVE_ACC_MMS2_DFLT).value<real>();
    axisXOperativeDecMms2 = settings.value(AXIS_X_OPERATIVE_DEC_MMS2, AXIS_X_OPERATIVE_DEC_MMS2_DFLT).value<real>();
    axisXHomingSpeedMms = settings.value(AXIS_X_HOMING_SPEED_MMS, AXIS_X_HOMING_SPEED_MMS_DFLT).value<real>();
    axisXHomingAccMms2 = settings.value(AXIS_X_HOMING_ACC_MMS2, AXIS_X_HOMING_ACC_MMS2_DFLT).value<real>();
    axisXHomingDecMms2 = settings.value(AXIS_X_HOMING_DEC_MMS2, AXIS_X_HOMING_DEC_MMS2_DFLT).value<real>();
    axisXCheckLimits = settings.value(AXIS_X_CHECK_LIMITS, AXIS_X_CHECK_LIMITS_DFLT).value<bool>();

    axisYStepPerMm = settings.value(AXIS_Y_STEP_PER_MM, AXIS_Y_STEP_PER_MM_DFLT).value<int>();
    axisYMinPosMm = settings.value(AXIS_Y_MIN_POS_MM, AXIS_Y_MIN_POS_MM_DFLT).value<int>();
    axisYMaxPosMm = settings.value(AXIS_Y_MAX_POS_MM, AXIS_Y_MAX_POS_MM_DFLT).value<int>();
    axisYBaseOffsetMm = settings.value(AXIS_Y_BASE_OFFSET_MM, AXIS_Y_BASE_OFFSET_MM_DFLT).value<real>();
    axisYManualSpeedMms = settings.value(AXIS_Y_MANUAL_SPEED_MMS, AXIS_Y_MANUAL_SPEED_MMS_DFLT).value<real>();
    axisYManualAccMms2 = settings.value(AXIS_Y_MANUAL_ACC_MMS2, AXIS_Y_MANUAL_ACC_MMS2_DFLT).value<real>();
    axisYManualDecMms2 = settings.value(AXIS_Y_MANUAL_DEC_MMS2, AXIS_Y_MANUAL_DEC_MMS2_DFLT).value<real>();
    axisYOperativeSpeedMms = settings.value(AXIS_Y_OPERATIVE_SPEED_MMS, AXIS_Y_OPERATIVE_SPEED_MMS_DFLT).value<real>();
    axisYOperativeAccMms2 = settings.value(AXIS_Y_OPERATIVE_ACC_MMS2, AXIS_Y_OPERATIVE_ACC_MMS2_DFLT).value<real>();
    axisYOperativeDecMms2 = settings.value(AXIS_Y_OPERATIVE_DEC_MMS2, AXIS_Y_OPERATIVE_DEC_MMS2_DFLT).value<real>();
    axisYHomingSpeedMms = settings.value(AXIS_Y_HOMING_SPEED_MMS, AXIS_Y_HOMING_SPEED_MMS_DFLT).value<real>();
    axisYHomingAccMms2 = settings.value(AXIS_Y_HOMING_ACC_MMS2, AXIS_Y_HOMING_ACC_MMS2_DFLT).value<real>();
    axisYHomingDecMms2 = settings.value(AXIS_Y_HOMING_DEC_MMS2, AXIS_Y_HOMING_DEC_MMS2_DFLT).value<real>();
    axisYCheckLimits = settings.value(AXIS_Y_CHECK_LIMITS, AXIS_Y_CHECK_LIMITS_DFLT).value<bool>();

    axisZStepPerMm = settings.value(AXIS_Z_STEP_PER_MM, AXIS_Z_STEP_PER_MM_DFLT).value<int>();
    axisZMinPosMm = settings.value(AXIS_Z_MIN_POS_MM, AXIS_Z_MIN_POS_MM_DFLT).value<int>();
    axisZMaxPosMm = settings.value(AXIS_Z_MAX_POS_MM, AXIS_Z_MAX_POS_MM_DFLT).value<int>();
    axisZBaseOffsetMm = settings.value(AXIS_Z_BASE_OFFSET_MM, AXIS_Z_BASE_OFFSET_MM_DFLT).value<real>();
    axisZManualSpeedMms = settings.value(AXIS_Z_MANUAL_SPEED_MMS, AXIS_Z_MANUAL_SPEED_MMS_DFLT).value<real>();
    axisZManualAccMms2 = settings.value(AXIS_Z_MANUAL_ACC_MMS2, AXIS_Z_MANUAL_ACC_MMS2_DFLT).value<real>();
    axisZManualDecMms2 = settings.value(AXIS_Z_MANUAL_DEC_MMS2, AXIS_Z_MANUAL_DEC_MMS2_DFLT).value<real>();
    axisZOperativeSpeedMms = settings.value(AXIS_Z_OPERATIVE_SPEED_MMS, AXIS_Z_OPERATIVE_SPEED_MMS_DFLT).value<real>();
    axisZOperativeAccMms2 = settings.value(AXIS_Z_OPERATIVE_ACC_MMS2, AXIS_Z_OPERATIVE_ACC_MMS2_DFLT).value<real>();
    axisZOperativeDecMms2 = settings.value(AXIS_Z_OPERATIVE_DEC_MMS2, AXIS_Z_OPERATIVE_DEC_MMS2_DFLT).value<real>();
    axisZHomingSpeedMms = settings.value(AXIS_Z_HOMING_SPEED_MMS, AXIS_Z_HOMING_SPEED_MMS_DFLT).value<real>();
    axisZHomingAccMms2 = settings.value(AXIS_Z_HOMING_ACC_MMS2, AXIS_Z_HOMING_ACC_MMS2_DFLT).value<real>();
    axisZHomingDecMms2 = settings.value(AXIS_Z_HOMING_DEC_MMS2, AXIS_Z_HOMING_DEC_MMS2_DFLT).value<real>();
    axisZCheckLimits = settings.value(AXIS_Z_CHECK_LIMITS, AXIS_Z_CHECK_LIMITS_DFLT).value<bool>();

    // lettura input digitali
    int size = settings.beginReadArray(Settings::ARRAY_DIGITAL_INPUT);
    for (int i=0; i<size; ++i) {
        settings.setArrayIndex(i);
        auto device = Utils::getDeviceKeyFromString(settings.value(DIGITAL_INPUT_DEVICE).value<QString>());
        auto channel = settings.value(DIGITAL_INPUT_CHANNEL).value<int>();
        if (channel == DIGITAL_INPUT_CHANNEL_NONE || device == DeviceKey::NONE)
            continue;
        auto type = Utils::getIOTypeFromString(settings.value(DIGITAL_INPUT_TYPE).value<QString>());
        QString name;
        if (type == IOType::GENERIC_INPUT)
            name = settings.value(DIGITAL_INPUT_NAME).value<QString>();
        else
            name = Utils::getIODescription(type);
        auto invertLogic = settings.value(DIGITAL_INPUT_INVERT_LOGIC).value<bool>();
        auto isAlarm = settings.value(DIGITAL_INPUT_IS_ALARM).value<bool>();
        auto isAlarmInverted = settings.value(DIGITAL_INPUT_IS_ALARM_INVERTED).value<bool>();
        DigitalInput input(name, channel, invertLogic, device, isAlarm, isAlarmInverted, type);
        this->digitalInputs.insertMulti(static_cast<IOType>(type), input);
    }
    settings.endArray();

    // per gli input obbligatori, controllo che siano presenti
    // FIXME non usare getIoDescription perche' usato per la UI
//    if (!digitalInputs.contains(IOType::POWER))
//        this->digitalInputs[IOType::POWER] = DigitalInput(Utils::getIODescription(IOType::POWER), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::GALIL_CN, false, IOType::POWER);
//    if (!digitalInputs.contains(IOType::CYCLE))
//        this->digitalInputs[IOType::CYCLE] = DigitalInput(Utils::getIODescription(IOType::CYCLE), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::GALIL_CN, false, IOType::CYCLE);
//    if (!digitalInputs.contains(IOType::EMERGENCY_MUSHROOM))
//        this->digitalInputs[IOType::EMERGENCY_MUSHROOM] = DigitalInput(Utils::getIODescription(IOType::EMERGENCY_MUSHROOM), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::EMERGENCY_MUSHROOM);
//    if (!digitalInputs.contains(IOType::DOOR))
//        this->digitalInputs[IOType::DOOR] = DigitalInput(Utils::getIODescription(IOType::DOOR), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::DOOR);
//    if (!digitalInputs.contains(IOType::BYPASS_SECURITY))
//        this->digitalInputs[IOType::BYPASS_SECURITY] = DigitalInput(Utils::getIODescription(IOType::BYPASS_SECURITY), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::BYPASS_SECURITY);
//    if (!digitalInputs.contains(IOType::WATER))
//        this->digitalInputs[IOType::WATER] = DigitalInput(Utils::getIODescription(IOType::WATER), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::WATER);
//    if (!digitalInputs.contains(IOType::MARK_IN_PROGRESS))
//        this->digitalInputs[IOType::MARK_IN_PROGRESS] = DigitalInput(Utils::getIODescription(IOType::MARK_IN_PROGRESS), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::MARK_IN_PROGRESS);
//    if (!digitalInputs.contains(IOType::SCANNER_READY))
//        this->digitalInputs[IOType::SCANNER_READY] = DigitalInput(Utils::getIODescription(IOType::SCANNER_READY), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::SCANNER_READY);
//    if (!digitalInputs.contains(IOType::SCANNER_ERROR))
//        this->digitalInputs[IOType::SCANNER_ERROR] = DigitalInput(Utils::getIODescription(IOType::SCANNER_ERROR), DIGITAL_INPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::SCANNER_ERROR);

    // lettura output digitali
    size = settings.beginReadArray(ARRAY_DIGITAL_OUTPUT);
    for (int i=0; i<size; ++i) {
        settings.setArrayIndex(i);
        auto device = Utils::getDeviceKeyFromString(settings.value(DIGITAL_INPUT_DEVICE).value<QString>());
        auto channel = settings.value(DIGITAL_OUTPUT_CHANNEL).value<int>();
        if (channel == DIGITAL_OUTPUT_CHANNEL_NONE || device == DeviceKey::NONE)
            continue;
        auto name = settings.value(DIGITAL_OUTPUT_NAME).value<QString>();
        auto type = Utils::getIOTypeFromString(settings.value(DIGITAL_INPUT_TYPE).value<QString>());
        auto invertLogic = settings.value(DIGITAL_OUTPUT_INVERT_LOGIC).value<bool>();
        auto isAlarm = settings.value(DIGITAL_OUTPUT_IS_ALARM).value<bool>();
        DigitalOutput output(name, channel, invertLogic, device, isAlarm, type);
        this->digitalOutputs[static_cast<IOType>(type)] = output;
    }
    settings.endArray();

    // controllo che siano presenti tutti gli output
//    if (!digitalOutputs.contains(IOType::LASER_POWER))
//        this->digitalOutputs[IOType::LASER_POWER] = DigitalOutput(Utils::getIODescription(IOType::LASER_POWER), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::LASER_POWER);
//    if (!digitalOutputs.contains(IOType::COMPRESSED_AIR_1))
//        this->digitalOutputs[IOType::COMPRESSED_AIR_1] = DigitalOutput(Utils::getIODescription(IOType::COMPRESSED_AIR_1), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::COMPRESSED_AIR_1);
//    if (!digitalOutputs.contains(IOType::COMPRESSED_AIR_2))
//        this->digitalOutputs[IOType::COMPRESSED_AIR_2] = DigitalOutput(Utils::getIODescription(IOType::COMPRESSED_AIR_2), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::COMPRESSED_AIR_2);
//    if (!digitalOutputs.contains(IOType::BRUSH_1))
//        this->digitalOutputs[IOType::BRUSH_1] = DigitalOutput(Utils::getIODescription(IOType::BRUSH_1), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::BRUSH_1);
//    if (!digitalOutputs.contains(IOType::BRUSH_2))
//        this->digitalOutputs[IOType::BRUSH_2] = DigitalOutput(Utils::getIODescription(IOType::BRUSH_2), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::BRUSH_2);
//    if (!digitalOutputs.contains(IOType::SUCTION))
//        this->digitalOutputs[IOType::SUCTION] = DigitalOutput(Utils::getIODescription(IOType::SUCTION), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::SUCTION);
//    if (!digitalOutputs.contains(IOType::ENABLE_AIR_FOR_FLUID))
//        this->digitalOutputs[IOType::ENABLE_AIR_FOR_FLUID] = DigitalOutput(Utils::getIODescription(IOType::ENABLE_AIR_FOR_FLUID), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::ENABLE_AIR_FOR_FLUID);
//    if (!digitalOutputs.contains(IOType::ENABLE_FLUID))
//        this->digitalOutputs[IOType::ENABLE_FLUID] = DigitalOutput(Utils::getIODescription(IOType::ENABLE_FLUID), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::ENABLE_FLUID);
//    if (!digitalOutputs.contains(IOType::POWER_SCAN))
//        this->digitalOutputs[IOType::POWER_SCAN] = DigitalOutput(Utils::getIODescription(IOType::POWER_SCAN), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::POWER_SCAN);
//    if (!digitalOutputs.contains(IOType::START_SCAN))
//        this->digitalOutputs[IOType::START_SCAN] = DigitalOutput(Utils::getIODescription(IOType::START_SCAN), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::START_SCAN);
//    if (!digitalOutputs.contains(IOType::STOP_SCAN))
//        this->digitalOutputs[IOType::STOP_SCAN] = DigitalOutput(Utils::getIODescription(IOType::STOP_SCAN), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::STOP_SCAN);
//    if (!digitalOutputs.contains(IOType::RED_LIGHT))
//        this->digitalOutputs[IOType::RED_LIGHT] = DigitalOutput(Utils::getIODescription(IOType::RED_LIGHT), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::RED_LIGHT);
//    if (!digitalOutputs.contains(IOType::GREEN_LIGHT))
//        this->digitalOutputs[IOType::GREEN_LIGHT] = DigitalOutput(Utils::getIODescription(IOType::GREEN_LIGHT), DIGITAL_OUTPUT_CHANNEL_NONE, false, DeviceKey::NONE, false, IOType::GREEN_LIGHT);

    // lettura input analogici
    size = settings.beginReadArray(Settings::ARRAY_ANALOG_INPUT);
    for (int i=size-1; i>=0; --i) {
        settings.setArrayIndex(i);
        auto name = settings.value(ANALOG_INPUT_NAME).value<QString>();
        auto channel = settings.value(ANALOG_INPUT_CHANNEL).value<int>();
        auto device = Utils::getDeviceKeyFromString(settings.value(ANALOG_INPUT_DEVICE).value<QString>());
        auto isAlarm = settings.value(ANALOG_INPUT_IS_ALARM).value<bool>();
        auto gain = settings.value(ANALOG_INPUT_GAIN).value<analogReal>();
        auto offset = settings.value(ANALOG_INPUT_OFFSET).value<analogReal>();
        auto unit = settings.value(ANALOG_INPUT_UNIT).value<QString>();
        auto lowerLimit = settings.value(ANALOG_INPUT_LOWER_LIMIT).value<analogReal>();
        auto upperLimit = settings.value(ANALOG_INPUT_UPPER_LIMIT).value<analogReal>();
        auto hysteresys = settings.value(ANALOG_INPUT_HYSTERESIS).value<analogReal>();
        AnalogInput aInput(name, channel, device, isAlarm, gain, offset, unit, lowerLimit, upperLimit, hysteresys);
        this->analogInputs.insertMulti(analogInputs.constEnd(), IOType::GENERIC_ANALOG_INPUT, aInput);
    }
    settings.endArray();

    // inizio lettura parametri di macchina

    machineCNType = Utils::getDeviceKeyFromString(settings.value(MACHINE_CN_TYPE, Utils::getStringFromDeviceKey(MACHINE_CN_TYPE_DFLT)).value<QString>());
    machinePLCType = Utils::getDeviceKeyFromString(settings.value(MACHINE_PLC_TYPE, Utils::getStringFromDeviceKey(MACHINE_PLC_TYPE_DFLT)).value<QString>());
    machineIORefreshIntervalMs = settings.value(MACHINE_IO_REFRESH_INTERVAL_MS, MACHINE_IO_REFRESH_INTERVAL_MS_DFLT).value<int>();

    // se il cn e' di tipo galil, carico i parametri del galil)
    if (DeviceKey::GALIL_CN == machineCNType) {
        galilCNConnectionTimeoutMs = settings.value(GALIL_CN_CONNECTION_TIMEOUT_MS, GALIL_CN_CONNECTION_TIMEOUT_MS_DFLT).value<int>();
        galilCNStatusRefreshIntervalMs = settings.value(GALIL_CN_STATUS_REFRESH_INTERVAL_MS, GALIL_CN_STATUS_REFRESH_INTERVAL_MS_DFLT).value<int>();
        galilCNCheckConnectionIntervalMs = settings.value(GALIL_CN_CHECK_CONNECTION_INTERVAL_MS, GALIL_CN_CHECK_CONNECTION_INTERVAL_MS_DFLT).value<int>();
        galilCNReconnectionIntervalMs = settings.value(GALIL_CN_RECONNECTION_INTERVAL_MS, GALIL_CN_RECONNECTION_INTERVAL_MS_DFLT).value<int>();
        galilCNNumberDigitalInput = settings.value(GALIL_CN_NUMBER_DIGITAL_INPUT, GALIL_CN_NUMBER_DIGITAL_INPUT_DFLT).value<int>();
        galilCNNumberDigitalOutput = settings.value(GALIL_CN_NUMBER_DIGITAL_OUTPUT, GALIL_CN_NUMBER_DIGITAL_OUTPUT_DFLT).value<int>();
        galilCNNumberAnalogInput = settings.value(GALIL_CN_NUMBER_ANALOG_INPUT, GALIL_CN_NUMBER_ANALOG_INPUT_DFLT).value<int>();
        galilCNNumberAnalogOutput = settings.value(GALIL_CN_NUMBER_ANALOG_OUTPUT, GALIL_CN_NUMBER_ANALOG_OUTPUT_DFLT).value<int>();
        galilCNIpAddress = settings.value(GALIL_CN_IP_ADDRESS, GALIL_CN_IP_ADDRESS_DFLT).value<QString>();
        galilCNOptionCustomHomeAxisX = settings.value(GALIL_CN_OPTION_CUSTOM_HOME_AXIS_X, GALIL_CN_OPTION_CUSTOM_HOME_AXIS_X_DFLT).value<bool>();
    }

    // se il plc e' di tipo galil, carico i parametri del galil)
    if (DeviceKey::GALIL_PLC == machinePLCType) {
        galilPLCConnectionTimeoutMs = settings.value(GALIL_PLC_CONNECTION_TIMEOUT_MS, GALIL_PLC_CONNECTION_TIMEOUT_MS_DFLT).value<int>();
        galilPLCStatusRefreshIntervalMs = settings.value(GALIL_PLC_STATUS_REFRESH_INTERVAL_MS, GALIL_PLC_STATUS_REFRESH_INTERVAL_MS_DFLT).value<int>();
        galilPLCCheckConnectionIntervalMs = settings.value(GALIL_PLC_CHECK_CONNECTION_INTERVAL_MS, GALIL_PLC_CHECK_CONNECTION_INTERVAL_MS_DFLT).value<int>();
        galilPLCReconnectionIntervalMs = settings.value(GALIL_PLC_RECONNECTION_INTERVAL_MS, GALIL_PLC_RECONNECTION_INTERVAL_MS_DFLT).value<int>();
        galilPLCNumberDigitalInput = settings.value(GALIL_PLC_NUMBER_DIGITAL_INPUT, GALIL_PLC_NUMBER_DIGITAL_INPUT_DFLT).value<int>();
        galilPLCNumberDigitalOutput = settings.value(GALIL_PLC_NUMBER_DIGITAL_OUTPUT, GALIL_PLC_NUMBER_DIGITAL_OUTPUT_DFLT).value<int>();
        galilPLCNumberAnalogInput = settings.value(GALIL_PLC_NUMBER_ANALOG_INPUT, GALIL_PLC_NUMBER_ANALOG_INPUT_DFLT).value<int>();
        galilPLCNumberAnalogOutput = settings.value(GALIL_PLC_NUMBER_ANALOG_OUTPUT, GALIL_PLC_NUMBER_ANALOG_OUTPUT_DFLT).value<int>();
        galilPLCIpAddress = settings.value(GALIL_PLC_IP_ADDRESS, GALIL_PLC_IP_ADDRESS_DFLT).value<QString>();
    }

    uiSpoolPath = settings.value(UI_SPOOL_PATH, UI_SPOOL_PATH_DFLT).value<QString>();
    markingConfigurationPath = settings.value(UI_MARKING_CONFIGURATION_PATH, UI_MARKING_CONFIGURATION_PATH_DFLT).value<QString>();

    traceDebug() << "axisXStepPerMm:" << axisXStepPerMm;
    traceDebug() << "axisXMinPosMm:" << axisXMinPosMm;
    traceDebug() << "axisXMaxPosMm:" << axisXMaxPosMm;
    traceDebug() << "axisXBaseOffsetMm:" << axisXBaseOffsetMm;
    traceDebug() << "axisXManualSpeedMms:" << axisXManualSpeedMms;
    traceDebug() << "axisXManualAccMms2:" << axisXManualAccMms2;
    traceDebug() << "axisXManualDecMms2:" << axisXManualDecMms2;
    traceDebug() << "axisXOperativeSpeedMms:" << axisXOperativeSpeedMms;
    traceDebug() << "axisXOperativeAccMms2:" << axisXOperativeAccMms2;
    traceDebug() << "axisXOperativeDecMms2:" << axisXOperativeDecMms2;

    traceDebug() << "axisYStepPerMm:" << axisYStepPerMm;
    traceDebug() << "axisYMinPosMm:" << axisYMinPosMm;
    traceDebug() << "axisYMaxPosMm:" << axisYMaxPosMm;
    traceDebug() << "axisYBaseOffsetMm:" << axisYBaseOffsetMm;
    traceDebug() << "axisYManualSpeedMms:" << axisYManualSpeedMms;
    traceDebug() << "axisYManualAccMms2:" << axisYManualAccMms2;
    traceDebug() << "axisYManualDecMms2:" << axisYManualDecMms2;
    traceDebug() << "axisYOperativeSpeedMms:" << axisYOperativeSpeedMms;
    traceDebug() << "axisYOperativeAccMms2:" << axisYOperativeAccMms2;
    traceDebug() << "axisYOperativeDecMms2:" << axisYOperativeDecMms2;

    traceDebug() << "axisZStepPerMm:" << axisZStepPerMm;
    traceDebug() << "axisZMinPosMm:" << axisZMinPosMm;
    traceDebug() << "axisZMaxPosMm:" << axisZMaxPosMm;
    traceDebug() << "axisZBaseOffsetMm:" << axisZBaseOffsetMm;
    traceDebug() << "axisZManualSpeedMms:" << axisZManualSpeedMms;
    traceDebug() << "axisZManualAccMms2:" << axisZManualAccMms2;
    traceDebug() << "axisZManualDecMms2:" << axisZManualDecMms2;
    traceDebug() << "axisZOperativeSpeedMms:" << axisZOperativeSpeedMms;
    traceDebug() << "axisZOperativeAccMms2:" << axisZOperativeAccMms2;
    traceDebug() << "axisZOperativeDecMms2:" << axisZOperativeDecMms2;

    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)
    ipgYLPNLaserIpAddress = settings.value(IPG_YLPN_ETHERNET_LASER_IP, IPG_YLPN_ETHERNET_LASER_IP_DFLT).value<QString>();
    ipgYLPNLaserPort = settings.value(IPG_YLPN_ETHERNET_LASER_PORT, IPG_YLPN_ETHERNET_LASER_PORT_DFLT).value<quint16>();

    traceExit;

}

void Settings::writeValuesToFile() {

    traceEnter;

    using namespace settings;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION, APPLICATION_NAME);

    settings.setValue(AXIS_X_STEP_PER_MM, static_cast<double>(axisXStepPerMm));
    settings.setValue(AXIS_X_MIN_POS_MM, static_cast<double>(axisXMinPosMm));
    settings.setValue(AXIS_X_MAX_POS_MM, static_cast<double>(axisXMaxPosMm));
    settings.setValue(AXIS_X_BASE_OFFSET_MM, static_cast<double>(axisXBaseOffsetMm));
    settings.setValue(AXIS_X_MANUAL_SPEED_MMS, static_cast<double>(axisXManualSpeedMms));
    settings.setValue(AXIS_X_MANUAL_ACC_MMS2, static_cast<double>(axisXManualAccMms2));
    settings.setValue(AXIS_X_MANUAL_DEC_MMS2, static_cast<double>(axisXManualDecMms2));
    settings.setValue(AXIS_X_OPERATIVE_SPEED_MMS, static_cast<double>(axisXOperativeSpeedMms));
    settings.setValue(AXIS_X_OPERATIVE_ACC_MMS2, static_cast<double>(axisXOperativeAccMms2));
    settings.setValue(AXIS_X_OPERATIVE_DEC_MMS2, static_cast<double>(axisXOperativeDecMms2));
    settings.setValue(AXIS_X_HOMING_SPEED_MMS, static_cast<double>(axisXHomingSpeedMms));
    settings.setValue(AXIS_X_HOMING_ACC_MMS2, static_cast<double>(axisXHomingAccMms2));
    settings.setValue(AXIS_X_HOMING_DEC_MMS2, static_cast<double>(axisXHomingDecMms2));
    settings.setValue(AXIS_X_CHECK_LIMITS, static_cast<bool>(axisXCheckLimits));

    settings.setValue(AXIS_Y_STEP_PER_MM, static_cast<double>(axisYStepPerMm));
    settings.setValue(AXIS_Y_MIN_POS_MM, static_cast<double>(axisYMinPosMm));
    settings.setValue(AXIS_Y_MAX_POS_MM, static_cast<double>(axisYMaxPosMm));
    settings.setValue(AXIS_Y_BASE_OFFSET_MM, static_cast<double>(axisYBaseOffsetMm));
    settings.setValue(AXIS_Y_MANUAL_SPEED_MMS, static_cast<double>(axisYManualSpeedMms));
    settings.setValue(AXIS_Y_MANUAL_ACC_MMS2, static_cast<double>(axisYManualAccMms2));
    settings.setValue(AXIS_Y_MANUAL_DEC_MMS2, static_cast<double>(axisYManualDecMms2));
    settings.setValue(AXIS_Y_OPERATIVE_SPEED_MMS, static_cast<double>(axisYOperativeSpeedMms));
    settings.setValue(AXIS_Y_OPERATIVE_ACC_MMS2, static_cast<double>(axisYOperativeAccMms2));
    settings.setValue(AXIS_Y_OPERATIVE_DEC_MMS2, static_cast<double>(axisYOperativeDecMms2));
    settings.setValue(AXIS_Y_HOMING_SPEED_MMS, static_cast<double>(axisYHomingSpeedMms));
    settings.setValue(AXIS_Y_HOMING_ACC_MMS2, static_cast<double>(axisYHomingAccMms2));
    settings.setValue(AXIS_Y_HOMING_DEC_MMS2, static_cast<double>(axisYHomingDecMms2));
    settings.setValue(AXIS_Y_CHECK_LIMITS, static_cast<bool>(axisYCheckLimits));

    settings.setValue(AXIS_Z_STEP_PER_MM, static_cast<double>(axisZStepPerMm));
    settings.setValue(AXIS_Z_MIN_POS_MM, static_cast<double>(axisZMinPosMm));
    settings.setValue(AXIS_Z_MAX_POS_MM, static_cast<double>(axisZMaxPosMm));
    settings.setValue(AXIS_Z_BASE_OFFSET_MM, static_cast<double>(axisZBaseOffsetMm));
    settings.setValue(AXIS_Z_MANUAL_SPEED_MMS, static_cast<double>(axisZManualSpeedMms));
    settings.setValue(AXIS_Z_MANUAL_ACC_MMS2, static_cast<double>(axisZManualAccMms2));
    settings.setValue(AXIS_Z_MANUAL_DEC_MMS2, static_cast<double>(axisZManualDecMms2));
    settings.setValue(AXIS_Z_OPERATIVE_SPEED_MMS, static_cast<double>(axisZOperativeSpeedMms));
    settings.setValue(AXIS_Z_OPERATIVE_ACC_MMS2, static_cast<double>(axisZOperativeAccMms2));
    settings.setValue(AXIS_Z_OPERATIVE_DEC_MMS2, static_cast<double>(axisZOperativeDecMms2));
    settings.setValue(AXIS_Z_HOMING_SPEED_MMS, static_cast<double>(axisZHomingSpeedMms));
    settings.setValue(AXIS_Z_HOMING_ACC_MMS2, static_cast<double>(axisZHomingAccMms2));
    settings.setValue(AXIS_Z_HOMING_DEC_MMS2, static_cast<double>(axisZHomingDecMms2));
    settings.setValue(AXIS_Z_CHECK_LIMITS, static_cast<bool>(axisZCheckLimits));

    // lettura input digitali
    settings.remove(Settings::ARRAY_DIGITAL_INPUT);
    settings.beginWriteArray(Settings::ARRAY_DIGITAL_INPUT);
    int i=0;
    for (DigitalInput input: digitalInputs) {
        settings.setArrayIndex(i++);
        settings.setValue(DIGITAL_INPUT_NAME, input.getName());
        settings.setValue(DIGITAL_INPUT_TYPE, Utils::getStringFromIOType(input.getElementType()));
        settings.setValue(DIGITAL_INPUT_CHANNEL, input.getChannel());
        settings.setValue(DIGITAL_INPUT_INVERT_LOGIC, input.getInvertLogic());
        settings.setValue(DIGITAL_INPUT_DEVICE, Utils::getStringFromDeviceKey(input.getDevice()));
        settings.setValue(DIGITAL_INPUT_IS_ALARM, input.getIsAlarm());
        settings.setValue(DIGITAL_INPUT_IS_ALARM_INVERTED, input.getIsAlarmInverted());
    }
    settings.endArray();

    // lettura output digitali
    settings.remove(Settings::ARRAY_DIGITAL_OUTPUT);
    settings.beginWriteArray(Settings::ARRAY_DIGITAL_OUTPUT);
    i=0;
    for (DigitalOutput output: digitalOutputs) {
        settings.setArrayIndex(i++);
        settings.setValue(DIGITAL_OUTPUT_NAME, output.getName());
        settings.setValue(DIGITAL_OUTPUT_TYPE, Utils::getStringFromIOType(output.getElementType()));
        settings.setValue(DIGITAL_OUTPUT_CHANNEL, output.getChannel());
        settings.setValue(DIGITAL_OUTPUT_INVERT_LOGIC, output.getInvertLogic());
        settings.setValue(DIGITAL_OUTPUT_DEVICE, Utils::getStringFromDeviceKey(output.getDevice()));
        settings.setValue(DIGITAL_OUTPUT_IS_ALARM, output.getIsAlarm());
    }
    settings.endArray();

    // lettura input analogici
    if (analogInputs.isEmpty()) {
        settings.remove(Settings::ARRAY_ANALOG_INPUT);
        settings.beginWriteArray(Settings::ARRAY_ANALOG_INPUT);
        i=0;
        settings.setArrayIndex(i++);
        AnalogInput sample;
        // se non ci sono input, ne scrivo uno di esempio
        settings.setValue(ANALOG_INPUT_NAME, sample.getName());
        settings.setValue(ANALOG_INPUT_CHANNEL, sample.getChannel());
        settings.setValue(ANALOG_INPUT_DEVICE, Utils::getStringFromDeviceKey(sample.getDevice()));
        settings.setValue(ANALOG_INPUT_IS_ALARM, sample.getIsAlarm());
        settings.setValue(ANALOG_INPUT_GAIN, sample.getGain());
        settings.setValue(ANALOG_INPUT_OFFSET, sample.getOffset());
        settings.setValue(ANALOG_INPUT_UNIT, sample.getUnit());
        settings.setValue(ANALOG_INPUT_LOWER_LIMIT, sample.getLowerLimit());
        settings.setValue(ANALOG_INPUT_UPPER_LIMIT, sample.getUpperLimit());
        settings.setValue(ANALOG_INPUT_HYSTERESIS, sample.getHysteresys());
        settings.endArray();
    }

    settings.setValue(MACHINE_CN_TYPE, Utils::getStringFromDeviceKey(machineCNType));
    settings.setValue(MACHINE_PLC_TYPE, Utils::getStringFromDeviceKey(machinePLCType));
    settings.setValue(MACHINE_IO_REFRESH_INTERVAL_MS, machineIORefreshIntervalMs);

    // se il cn e' di tipo galil, carico i parametri del galil)
    if (DeviceKey::GALIL_CN == machineCNType) {
        settings.setValue(GALIL_CN_CONNECTION_TIMEOUT_MS, galilCNConnectionTimeoutMs);
        settings.setValue(GALIL_CN_STATUS_REFRESH_INTERVAL_MS, galilCNStatusRefreshIntervalMs);
        settings.setValue(GALIL_CN_CHECK_CONNECTION_INTERVAL_MS, galilCNCheckConnectionIntervalMs);
        settings.setValue(GALIL_CN_RECONNECTION_INTERVAL_MS, galilCNReconnectionIntervalMs);
        settings.setValue(GALIL_CN_IP_ADDRESS, galilCNIpAddress);
        settings.setValue(GALIL_CN_NUMBER_DIGITAL_INPUT, galilCNNumberDigitalInput);
        settings.setValue(GALIL_CN_NUMBER_DIGITAL_OUTPUT, galilCNNumberDigitalOutput);
        settings.setValue(GALIL_CN_NUMBER_ANALOG_INPUT, galilCNNumberAnalogInput);
        settings.setValue(GALIL_CN_NUMBER_ANALOG_OUTPUT, galilCNNumberAnalogOutput);
        settings.setValue(GALIL_CN_OPTION_CUSTOM_HOME_AXIS_X, galilCNOptionCustomHomeAxisX);
    } else
        settings.remove(GALIL_CN_PREFIX);


    // se il plc e' di tipo galil, carico i parametri del galil)
    if (DeviceKey::GALIL_PLC == machinePLCType) {
        settings.setValue(GALIL_PLC_CONNECTION_TIMEOUT_MS, galilPLCConnectionTimeoutMs);
        settings.setValue(GALIL_PLC_STATUS_REFRESH_INTERVAL_MS, galilPLCStatusRefreshIntervalMs);
        settings.setValue(GALIL_PLC_CHECK_CONNECTION_INTERVAL_MS, galilPLCCheckConnectionIntervalMs);
        settings.setValue(GALIL_PLC_RECONNECTION_INTERVAL_MS, galilPLCReconnectionIntervalMs);
        settings.setValue(GALIL_PLC_IP_ADDRESS, galilPLCIpAddress);
        settings.setValue(GALIL_PLC_NUMBER_DIGITAL_INPUT, galilPLCNumberDigitalInput);
        settings.setValue(GALIL_PLC_NUMBER_DIGITAL_OUTPUT, galilPLCNumberDigitalOutput);
        settings.setValue(GALIL_PLC_NUMBER_ANALOG_INPUT, galilPLCNumberAnalogInput);
        settings.setValue(GALIL_PLC_NUMBER_ANALOG_OUTPUT, galilPLCNumberAnalogOutput);
    } else
        settings.remove(GALIL_PLC_PREFIX);

    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)
    settings.setValue(IPG_YLPN_ETHERNET_LASER_IP, ipgYLPNLaserIpAddress);
    settings.setValue(IPG_YLPN_ETHERNET_LASER_PORT, ipgYLPNLaserPort);

    settings.setValue(UI_SPOOL_PATH, uiSpoolPath);
    settings.setValue(UI_MARKING_CONFIGURATION_PATH, markingConfigurationPath);

    settings.sync();

    traceExit;

}

bool Settings::validateDigitalInput(const DigitalInput& d) const {

    traceEnter;

    bool isOk = true;
    if (d.getName().isEmpty()) {
        traceErr() << "Digital Input: nome non presente";
        isOk = false;
    }

    if (d.getDevice() < DeviceKey::GALIL_CN || d.getDevice() > DeviceKey::GALIL_PLC) {
        traceErr() << "Digital Input: device sconosciuto";
        isOk = false;
    }

    if (d.getChannel() > 0)
        if (d.getElementType() < IOType::POWER || d.getElementType() > IOType::GENERIC_INPUT) {
            traceErr() << "Digital Input: tipo di elemento sconosciuto";
            isOk = false;
        }

    traceExit;
    return isOk;

}

bool Settings::validateDigitalOutput(const DigitalOutput& d) const {

    traceEnter;

    bool isOk = true;
    if (d.getName().isEmpty()) {
        traceErr() << "Digital Output: nome non presente";
        isOk = false;
    }

    if (d.getDevice() < DeviceKey::GALIL_CN || d.getDevice() > DeviceKey::GALIL_PLC) {
        traceErr() << "Digital Output: device sconosciuto";
        isOk = false;
    }

    if (d.getChannel() > 0)
        if (d.getElementType() < IOType::LASER_POWER || d.getElementType() > IOType::GREEN_LIGHT) {
            traceErr() << "Digital Output: tipo di elemento sconosciuto";
            isOk = false;
        }

    traceExit;
    return isOk;

}

bool Settings::validateAnalogInput(const AnalogInput& a) const {

    traceEnter;
    bool isOk = true;
    if (a.getName().isEmpty()) {
        traceErr() << "Analog Input: nome non presente";
        isOk = false;
    }

    if (a.getDevice() < DeviceKey::GALIL_CN || a.getDevice() > DeviceKey::GALIL_PLC) {
        traceErr() << "Analog Input: device sconosciuto";
        isOk = false;
    }

    traceExit;
    return isOk;

}

bool Settings::validateSettings() const {

    traceEnter;

    /* NOTE NIC 30/10/2018 - per ora il controllo delle analogiche
     * viene fatto solamente sul PLC
     */
    // TODO NIC 15/12/2018 - controllare che gli ingressi POWER e CYCLE siano presenti

    // key = ingresso
    // value = chiave ridondante
    QMap<int, IOType> digitalInputCN;
    QMap<int, IOType> digitalOutputCN;
    QMap<int, IOType> digitalInputPLC;
    QMap<int, IOType> digitalOutputPLC;
    QMap<int, IOType> analogInputPLC;

    bool isValid = true;
    for (DigitalInput i: digitalInputs) {
        bool temp = validateDigitalInput(i);
        if (!temp)
            isValid = false;
        if (i.getChannel() == DIGITAL_INPUT_CHANNEL_NONE)
            continue;
        if (i.getDevice() == DeviceKey::GALIL_CN)
            digitalInputCN.insertMulti(i.getChannel(), i.getElementType());
        else
            digitalInputPLC.insertMulti(i.getChannel(), i.getElementType());
    }

    for (DigitalOutput o: digitalOutputs) {
        bool temp = validateDigitalOutput(o);
        if (!temp)
            isValid = false;
        if (o.getChannel() == DIGITAL_OUTPUT_CHANNEL_NONE)
            continue;
        if (o.getDevice() == DeviceKey::GALIL_CN)
            digitalOutputCN.insertMulti(o.getChannel(), o.getElementType());
        else
            digitalOutputPLC.insertMulti(o.getChannel(), o.getElementType());
    }

    for (AnalogInput a: analogInputs) {
        bool temp = validateAnalogInput(a);
        if (!temp)
            isValid = false;
        if (a.getChannel() == ANALOG_INPUT_CHANNEL_NONE)
            continue;
        analogInputPLC.insertMulti(a.getChannel(), a.getElementType());
    }

    // verifico se ci sono ingressi 'doppi'
    for (int i: digitalInputCN.keys())
        if (digitalInputCN.values(i).size()>1) {
            traceErr() << "Errore IO per Galil CN: piu' dispositivi collegati all'ingresso" << i;
            isValid = false;
        }

    for (int i: digitalInputPLC.keys())
        if (digitalInputPLC.values(i).size()>1) {
            traceErr() << "Errore IO per Galil PLC: piu' dispositivi collegati all'uscita" << i;
            isValid = false;
        }

    for (int i: digitalOutputCN.keys())
        if (digitalOutputCN.values(i).size()>1) {
            traceErr() << "Errore IO per Galil CN: piu' dispositivi collegati all'ingresso" << i;
            isValid = false;
        }

    for (int i: digitalOutputPLC.keys())
        if (digitalOutputPLC.values(i).size()>1) {
            traceErr() << "Errore IO per Galil PLC piu' dispositivi collegati all'uscita" << i;
            isValid = false;
        }

    for (int i: analogInputPLC.keys())
        if (analogInputPLC.values(i).size()>1) {
            traceErr() << "Errore IO per Galil PLC: piu' dispositivi collegati all'ingresso analogico" << i;
            isValid = false;
        }

    traceExit;

    return isValid;

}

Settings& Settings::instance() {

    static Settings instance;
    return instance;

}
