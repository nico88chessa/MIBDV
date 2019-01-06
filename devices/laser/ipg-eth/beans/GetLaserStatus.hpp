#ifndef GET_LASER_STATUS_HPP
#define GET_LASER_STATUS_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

typedef InputBean GetLaserStatusInput;

class GetLaserStatusOutput : public OutputBean {
public:
    typedef GetLaserStatusOutput* Ptr;
    typedef const GetLaserStatusOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_ULONG installedStatus;
    IPG_USHORT modeIndex;
    IPG_FLOAT frequency;
    IPG_FLOAT opticalPowerWatt;
    IPG_FLOAT opticalPowerPerc;
    IPG_ULONG mainStatus;
    IPG_ULONG alarms;           // vedere bit alarm
    IPG_ULONG systemAlarms;     // vedere bit system alarm
    IPG_ULONG extendedAlarms;   // vedere bit extended alarm
    IPG_ULONG warnings;         // vedere bit warning
    IPG_ULONG status;           // vedere bit status
    IPG_ULONG extendedStatus;   // vedere bit extended status
    IPG_USHORT reserved1;
    IPG_FLOAT measureFrequency;
    IPG_ULONG pulseDuration;
    IPG_FLOAT measurePower;
    IPG_FLOAT pulseEnergy;
    IPG_FLOAT peakPower;
    IPG_FLOAT analogPowerControl;
    IPG_FLOAT backReflection;
    IPG_FLOAT powerSupplyTemp;
    IPG_ULONG reserved2;
    IPG_FLOAT interfaceControllerTemp;
    IPG_ULONG reserved3;
    IPG_FLOAT opticalModule1Temp;
    IPG_FLOAT opticalModule2Temp;
    IPG_ULONG reserved4;

    // flag boolean

    bool installedStatusPowerSupply;
    bool installedStatusEmission;
    bool installedStatusModulation;
    bool installedStatusGuideLaser;

    bool mainStatusAlarms;
    bool mainStatusWarnings;

    bool alarmSystemAlarm;
    bool alarmCriticalAlarm;
    bool alarmTemperatureAlarm;
    bool alarmInterfaceController24vAlarm;
    bool alarmBackReflectionAlarm;
    bool alarmOpticalLinkAlarm;
    bool alarmProcessFiberAlarm;

    bool systemAlarmLaserInterface;
    bool systemAlarmOpticalModule1;
    bool systemAlarmOpticalModule2;
    bool systemAlarmPowerSupply;
    bool systemAlarmSafety;
    bool systemAlarmCommunicationOm1;
    bool systemAlarmCommunicationOm2;
    bool systemAlarmCommunicationPs;
    bool systemAlarmCommunicationSafety;
    bool systemAlarmFeedingFiberAlarm;

    bool extendedAlarmInterfaceVoltageTooLow;
    bool extendedAlarmInterfaceVoltageTooHigh;
    bool extendedAlarmInterfaceTemperatureTooLow;
    bool extendedAlarmInterfaceTemperatureTooHigh;
    bool extendedAlarmLaserTemperatureTooLow;
    bool extendedAlarmLaserTemperatureTooHigh;
    bool extendedAlarmPsTemperatureTooHigh;

    bool warningsPrrLow;
    bool warningsPrrHigh;
    bool warningsSdcardFailure;
    bool warningsInterfaceTemperatureTooLow;
    bool warningsInterfaceTemperatureTooHigh;
    bool warningsLaserTemperatureTooLow;
    bool warningsLaserTemperatureTooHigh;

    bool statusSafetyCircuitOpen;
    bool statusPowerSupplyIsOn;
    bool statusLaserReady;
    bool statusEmissionIsOn;
    bool statusGuideLaser;
    bool statusExternalStop;
    bool statusSafetyCircuitBlocked;
    bool statusHotStop;

    bool extendedStatusInterfacePsHotStop;
    bool extendedStatusMainPsHotStop;
    bool extendedStatusOpticalLinkHotStop;
    bool extendedStatusSafetyOpenWhileEmissionStop;
    bool extendedStatusExternalStopWhileEmission;
    bool extendedStatusGlWhileEmissionLow;

public:

    IPG_ULONG getInstalledStatus() const { return installedStatus; }

    void setInstalledStatus(const IPG_ULONG& value) { installedStatus = value; }

    IPG_USHORT getModeIndex() const { return modeIndex; }

    void setModeIndex(const IPG_USHORT& value) { modeIndex = value; }

    IPG_FLOAT getFrequency() const { return frequency; }

    void setFrequency(const IPG_FLOAT& value) { frequency = value; }

    IPG_FLOAT getOpticalPowerWatt() const { return opticalPowerWatt; }

    void setOpticalPowerWatt(const IPG_FLOAT& value) { opticalPowerWatt = value; }

    IPG_FLOAT getOpticalPowerPerc() const { return opticalPowerPerc; }

    void setOpticalPowerPerc(const IPG_FLOAT& value) { opticalPowerPerc = value; }

    IPG_ULONG getMainStatus() const { return mainStatus; }

    void setMainStatus(const IPG_ULONG& value) { mainStatus = value; }

    IPG_ULONG getAlarms() const { return alarms; }

    void setAlarms(const IPG_ULONG& value) { alarms = value; }

    IPG_ULONG getSystemAlarms() const { return systemAlarms; }

    void setSystemAlarms(const IPG_ULONG& value) { systemAlarms = value; }

    IPG_ULONG getExtendedAlarms() const { return extendedAlarms; }

    void setExtendedAlarms(const IPG_ULONG& value) { extendedAlarms = value; }

    IPG_ULONG getWarnings() const { return warnings; }

    void setWarnings(const IPG_ULONG& value) { warnings = value; }

    IPG_ULONG getStatus() const { return status; }

    void setStatus(const IPG_ULONG& value) { status = value; }

    IPG_ULONG getExtendedStatus() const { return extendedStatus; }

    void setExtendedStatus(const IPG_ULONG& value) { extendedStatus = value; }

    IPG_USHORT getReserved1() const { return reserved1; }

    void setReserved1(const IPG_USHORT& value) { reserved1 = value; }

    IPG_FLOAT getMeasureFrequency() const { return measureFrequency; }

    void setMeasureFrequency(const IPG_FLOAT& value) { measureFrequency = value; }

    IPG_ULONG getPulseDuration() const { return pulseDuration; }

    void setPulseDuration(const IPG_ULONG& value) { pulseDuration = value; }

    IPG_FLOAT getMeasurePower() const { return measurePower; }

    void setMeasurePower(const IPG_FLOAT& value) { measurePower = value; }

    IPG_FLOAT getPulseEnergy() const { return pulseEnergy; }

    void setPulseEnergy(const IPG_FLOAT& value) { pulseEnergy = value; }

    IPG_FLOAT getPeakPower() const { return peakPower; }

    void setPeakPower(const IPG_FLOAT& value) { peakPower = value; }

    IPG_FLOAT getAnalogPowerControl() const { return analogPowerControl; }

    void setAnalogPowerControl(const IPG_FLOAT& value) { analogPowerControl = value; }

    IPG_FLOAT getBackReflection() const { return backReflection; }

    void setBackReflection(const IPG_FLOAT& value) { backReflection = value; }

    IPG_FLOAT getPowerSupplyTemp() const { return powerSupplyTemp; }

    void setPowerSupplyTemp(const IPG_FLOAT& value) { powerSupplyTemp = value; }

    IPG_ULONG getReserved2() const { return reserved2; }

    void setReserved2(const IPG_ULONG& value) { reserved2 = value; }

    IPG_FLOAT getInterfaceControllerTemp() const { return interfaceControllerTemp; }

    void setInterfaceControllerTemp(const IPG_FLOAT& value) { interfaceControllerTemp = value; }

    IPG_ULONG getReserved3() const { return reserved3; }

    void setReserved3(const IPG_ULONG& value) { reserved3 = value; }

    IPG_FLOAT getOpticalModule1Temp() const { return opticalModule1Temp; }

    void setOpticalModule1Temp(const IPG_FLOAT& value) { opticalModule1Temp = value; }

    IPG_FLOAT getOpticalModule2Temp() const { return opticalModule2Temp; }

    void setOpticalModule2Temp(const IPG_FLOAT& value) { opticalModule2Temp = value; }

    IPG_ULONG getReserved4() const { return reserved4; }

    void setReserved4(const IPG_ULONG& value) { reserved4 = value; }

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    // metodi per flag allarmi e warning

    bool getInstalledStatusEmission() const { return installedStatusEmission; }

    void setInstalledStatusEmission(bool value) { installedStatusEmission = value; }

    bool getInstalledStatusModulation() const { return installedStatusModulation; }

    void setInstalledStatusModulation(bool value) { installedStatusModulation = value; }

    bool getInstalledStatusGuideLaser() const { return installedStatusGuideLaser; }

    void setInstalledStatusGuideLaser(bool value) { installedStatusGuideLaser = value; }

    bool getInstalledStatusPowerSupply() const { return installedStatusPowerSupply; }

    void setInstalledStatusPowerSupply(bool value) { installedStatusPowerSupply = value; }

    bool getMainStatusWarnings() const { return mainStatusWarnings; }

    void setMainStatusWarnings(bool value) { mainStatusWarnings = value; }

    bool getMainStatusAlarms() const { return mainStatusAlarms; }

    void setMainStatusAlarms(bool value) { mainStatusAlarms = value; }

    bool getAlarmCriticalAlarm() const { return alarmCriticalAlarm; }

    void setAlarmCriticalAlarm(bool value) { alarmCriticalAlarm = value; }

    bool getAlarmTemperatureAlarm() const { return alarmTemperatureAlarm; }

    void setAlarmTemperatureAlarm(bool value) { alarmTemperatureAlarm = value; }

    bool getAlarmInterfaceController24vAlarm() const { return alarmInterfaceController24vAlarm; }

    void setAlarmInterfaceController24vAlarm(bool value) { alarmInterfaceController24vAlarm = value; }

    bool getAlarmBackReflectionAlarm() const { return alarmBackReflectionAlarm; }

    void setAlarmBackReflectionAlarm(bool value) { alarmBackReflectionAlarm = value; }

    bool getAlarmOpticalLinkAlarm() const { return alarmOpticalLinkAlarm; }

    void setAlarmOpticalLinkAlarm(bool value) { alarmOpticalLinkAlarm = value; }

    bool getAlarmProcessFiberAlarm() const { return alarmProcessFiberAlarm; }

    void setAlarmProcessFiberAlarm(bool value) { alarmProcessFiberAlarm = value; }

    bool getSystemAlarmLaserInterface() const { return systemAlarmLaserInterface; }

    void setSystemAlarmLaserInterface(bool value) { systemAlarmLaserInterface = value; }

    bool getSystemAlarmOpticalModule1() const { return systemAlarmOpticalModule1; }

    void setSystemAlarmOpticalModule1(bool value) { systemAlarmOpticalModule1 = value; }

    bool getSystemAlarmOpticalModule2() const { return systemAlarmOpticalModule2; }

    void setSystemAlarmOpticalModule2(bool value) { systemAlarmOpticalModule2 = value; }

    bool getSystemAlarmPowerSupply() const { return systemAlarmPowerSupply; }

    void setSystemAlarmPowerSupply(bool value) { systemAlarmPowerSupply = value; }

    bool getSystemAlarmSafety() const { return systemAlarmSafety; }

    void setSystemAlarmSafety(bool value) { systemAlarmSafety = value; }

    bool getSystemAlarmCommunicationOm1() const { return systemAlarmCommunicationOm1; }

    void setSystemAlarmCommunicationOm1(bool value) { systemAlarmCommunicationOm1 = value; }

    bool getSystemAlarmCommunicationOm2() const { return systemAlarmCommunicationOm2; }

    void setSystemAlarmCommunicationOm2(bool value) { systemAlarmCommunicationOm2 = value; }

    bool getSystemAlarmCommunicationPs() const { return systemAlarmCommunicationPs; }

    void setSystemAlarmCommunicationPs(bool value) { systemAlarmCommunicationPs = value; }

    bool getSystemAlarmCommunicationSafety() const { return systemAlarmCommunicationSafety; }

    void setSystemAlarmCommunicationSafety(bool value) { systemAlarmCommunicationSafety = value; }

    bool getSystemAlarmFeedingFiberAlarm() const { return systemAlarmFeedingFiberAlarm; }

    void setSystemAlarmFeedingFiberAlarm(bool value) { systemAlarmFeedingFiberAlarm = value; }

    bool getExtendedAlarmInterfaceVoltageTooLow() const { return extendedAlarmInterfaceVoltageTooLow; }

    void setExtendedAlarmInterfaceVoltageTooLow(bool value) { extendedAlarmInterfaceVoltageTooLow = value; }

    bool getExtendedAlarmInterfaceVoltageTooHigh() const { return extendedAlarmInterfaceVoltageTooHigh; }

    void setExtendedAlarmInterfaceVoltageTooHigh(bool value) { extendedAlarmInterfaceVoltageTooHigh = value; }

    bool getExtendedAlarmInterfaceTemperatureTooLow() const { return extendedAlarmInterfaceTemperatureTooLow; }

    void setExtendedAlarmInterfaceTemperatureTooLow(bool value) { extendedAlarmInterfaceTemperatureTooLow = value; }

    bool getExtendedAlarmInterfaceTemperatureTooHigh() const { return extendedAlarmInterfaceTemperatureTooHigh; }

    void setExtendedAlarmInterfaceTemperatureTooHigh(bool value) { extendedAlarmInterfaceTemperatureTooHigh = value; }

    bool getExtendedAlarmLaserTemperatureTooLow() const { return extendedAlarmLaserTemperatureTooLow; }

    void setExtendedAlarmLaserTemperatureTooLow(bool value) { extendedAlarmLaserTemperatureTooLow = value; }

    bool getExtendedAlarmLaserTemperatureTooHigh() const { return extendedAlarmLaserTemperatureTooHigh; }

    void setExtendedAlarmLaserTemperatureTooHigh(bool value) { extendedAlarmLaserTemperatureTooHigh = value; }

    bool getExtendedAlarmPsTemperatureTooHigh() const { return extendedAlarmPsTemperatureTooHigh; }

    void setExtendedAlarmPsTemperatureTooHigh(bool value) { extendedAlarmPsTemperatureTooHigh = value; }

    bool getWarningsPrrLow() const { return warningsPrrLow; }

    void setWarningsPrrLow(bool value) { warningsPrrLow = value; }

    bool getWarningsPrrHigh() const { return warningsPrrHigh; }

    void setWarningsPrrHigh(bool value) { warningsPrrHigh = value; }

    bool getWarningsSdcardFailure() const { return warningsSdcardFailure; }

    void setWarningsSdcardFailure(bool value) { warningsSdcardFailure = value; }

    bool getWarningsInterfaceTemperatureTooLow() const { return warningsInterfaceTemperatureTooLow; }

    void setWarningsInterfaceTemperatureTooLow(bool value) { warningsInterfaceTemperatureTooLow = value; }

    bool getWarningsInterfaceTemperatureTooHigh() const { return warningsInterfaceTemperatureTooHigh; }

    void setWarningsInterfaceTemperatureTooHigh(bool value) { warningsInterfaceTemperatureTooHigh = value; }

    bool getWarningsLaserTemperatureTooLow() const { return warningsLaserTemperatureTooLow; }

    void setWarningsLaserTemperatureTooLow(bool value) { warningsLaserTemperatureTooLow = value; }

    bool getWarningsLaserTemperatureTooHigh() const { return warningsLaserTemperatureTooHigh; }

    void setWarningsLaserTemperatureTooHigh(bool value) { warningsLaserTemperatureTooHigh = value; }

    bool getStatusSafetyCircuitOpen() const { return statusSafetyCircuitOpen; }

    void setStatusSafetyCircuitOpen(bool value) { statusSafetyCircuitOpen = value; }

    bool getStatusPowerSupplyIsOn() const { return statusPowerSupplyIsOn; }

    void setStatusPowerSupplyIsOn(bool value) { statusPowerSupplyIsOn = value; }

    bool getStatusLaserReady() const { return statusLaserReady; }

    void setStatusLaserReady(bool value) { statusLaserReady = value; }

    bool getStatusEmissionIsOn() const { return statusEmissionIsOn; }

    void setStatusEmissionIsOn(bool value) { statusEmissionIsOn = value; }

    bool getStatusGuideLaser() const { return statusGuideLaser; }

    void setStatusGuideLaser(bool value) { statusGuideLaser = value; }

    bool getStatusExternalStop() const { return statusExternalStop; }

    void setStatusExternalStop(bool value) { statusExternalStop = value; }

    bool getStatusSafetyCircuitBlocked() const { return statusSafetyCircuitBlocked; }

    void setStatusSafetyCircuitBlocked(bool value) { statusSafetyCircuitBlocked = value; }

    bool getStatusHotStop() const { return statusHotStop; }

    void setStatusHotStop(bool value) { statusHotStop = value; }

    bool getExtendedStatusInterfacePsHotStop() const { return extendedStatusInterfacePsHotStop; }

    void setExtendedStatusInterfacePsHotStop(bool value) { extendedStatusInterfacePsHotStop = value; }

    bool getExtendedStatusMainPsHotStop() const { return extendedStatusMainPsHotStop; }

    void setExtendedStatusMainPsHotStop(bool value) { extendedStatusMainPsHotStop = value; }

    bool getExtendedStatusOpticalLinkHotStop() const { return extendedStatusOpticalLinkHotStop; }

    void setExtendedStatusOpticalLinkHotStop(bool value) { extendedStatusOpticalLinkHotStop = value; }

    bool getExtendedStatusSafetyOpenWhileEmissionStop() const { return extendedStatusSafetyOpenWhileEmissionStop; }

    void setExtendedStatusSafetyOpenWhileEmissionStop(bool value) { extendedStatusSafetyOpenWhileEmissionStop = value; }

    bool getExtendedStatusExternalStopWhileEmission() const { return extendedStatusExternalStopWhileEmission; }

    void setExtendedStatusExternalStopWhileEmission(bool value) { extendedStatusExternalStopWhileEmission = value; }

    bool getExtendedStatusGlWhileEmissionLow() const { return extendedStatusGlWhileEmissionLow; }

    void setExtendedStatusGlWhileEmissionLow(bool value) { extendedStatusGlWhileEmissionLow = value; }

    bool getAlarmSystemAlarm() const { return alarmSystemAlarm; }

    void setAlarmSystemAlarm(bool value) { alarmSystemAlarm = value; }

};


}

Q_DECLARE_METATYPE(ipg::GetLaserStatusOutput)

#endif // GET_LASER_STATUS_HPP
