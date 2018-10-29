#include "Settings.hpp"

using namespace PROGRAM_NAMESPACE;

Settings::Settings() {

    traceEnter;

    this->loadValuesFromFile();
    this->writeValuesToFile();

    traceExit;

}

void Settings::loadValuesFromFile() {

    traceEnter;

    using namespace settings;

    QSettings settings(ORGANIZATION, APPLICATION_NAME);

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

    // lettura input digitali
    int size = settings.beginReadArray("DigitalInput");
    for (int i=0; i<size; ++i) {
        settings.setArrayIndex(i);
        auto name = settings.value(DIGITAL_INPUT_NAME).value<QString>();
        auto type = settings.value(DIGITAL_INPUT_TYPE).value<int>();
        auto channel = settings.value(DIGITAL_INPUT_CHANNEL).value<int>();
        auto invertLogic = settings.value(DIGITAL_INPUT_INVERT_LOGIC).value<bool>();
        auto device = settings.value(DIGITAL_INPUT_DEVICE).value<int>();
        auto isAlarm = settings.value(DIGITAL_INPUT_IS_ALARM).value<bool>();
        DigitalInput input(name, channel, invertLogic, static_cast<DeviceKey>(device), isAlarm, static_cast<IOType>(type));
        this->digitalInputs[static_cast<IOType>(type)] = input;
    }
    settings.endArray();

    // lettura output digitali
    settings.beginReadArray("DigitalOutput");
    for (int i=0; i<size; ++i) {
        settings.setArrayIndex(i);
//    for (int s = static_cast<int>(IOType::LASER_POWER); s <= static_cast<int>(IOType::GREEN_LIGHT); ++s) {
//        auto name = settings.value(DIGITAL_OUTPUT_NAME, Utils::getIODescription(static_cast<IOType>(s))).value<QString>();
//        auto type = settings.value(DIGITAL_OUTPUT_TYPE, Utils::getStringFromIOType(static_cast<IOType>(s))).value<QString>();
//        auto channel = settings.value(DIGITAL_OUTPUT_CHANNEL, -1).value<int>();
//        auto invertLogic = settings.value(DIGITAL_OUTPUT_INVERT_LOGIC, false).value<bool>();
//        auto device = settings.value(DIGITAL_OUTPUT_DEVICE, static_cast<int>(DeviceKey::GALIL_CN)).value<int>();
//        auto isAlarm = settings.value(DIGITAL_OUTPUT_IS_ALARM, false).value<bool>();
//        DigitalOutput output(name, channel, invertLogic, static_cast<DeviceKey>(device), isAlarm, static_cast<IOType>(s));
//        this->digitalOutputs[static_cast<IOType>(s)] = output;
    }
    settings.endArray();

    // lettura input analogici
    settings.beginReadArray("AnalogicInput");
//    for (int s = static_cast<int>(IOType::LASER_POWER); s<static_cast<int>(IOType::ANAL_AUTOFOCUS_DISTANCE); ++s) {
//        settings.value(ANALOGC_INPUT_NAME, Utils::getIODescription(static_cast<IOType>(s))).value<QString>();
//        settings.value(ANALOGC_INPUT_TYPE, Utils::getStringFromIOType(static_cast<IOType>(s))).value<QString>();
//        settings.value(ANALOGC_INPUT_CHANNEL, -1).value<int>();
//        settings.value(ANALOGC_INPUT_DEVICE, static_cast<int>(DeviceKey::GALIL_PLC)).value<int>();
//        settings.value(ANALOGC_INPUT_IS_ALARM, false).value<bool>();
//        settings.value(ANALOGC_INPUT_GAIN, 1).value<real>();
//        settings.value(ANALOGC_INPUT_OFFSET, 1).value<real>();
//        settings.value(ANALOGC_INPUT_UNIT, "").value<QString>();
//        settings.value(ANALOGC_INPUT_LOWER_LIMIT, 0).value<real>();
//        settings.value(ANALOGC_INPUT_UPPER_LIMIT, 0).value<real>();
//        settings.value(ANALOGC_INPUT_HYSTERESIS, 1.0).value<real>();
//    }
    settings.endArray();

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

    settings.sync();

    traceExit;
}

Settings& Settings::instance() {

    static Settings instance;
    return instance;

}

