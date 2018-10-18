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
