#ifndef IPG_LASER_TEMP_GET_LASER_STATUS_MARSHALLER_HPP
#define IPG_LASER_TEMP_GET_LASER_STATUS_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/get_laser_status.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class GetLaserStatusMarshaller : public IpgMarshaller<GetLaserStatusInput, GetLaserStatusOutput> {
public:
    typedef GetLaserStatusMarshaller* Ptr;
    typedef const GetLaserStatusMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetLaserStatusOutput::Ptr output) {

        /*
        const int offsets[29] = {
            0,
            2,
            6,
            8,
            12,
            16,
            20,
            24,
            28,
            32,
            36,
            40,
            44,
            48,
            50,
            54,
            58,
            62,
            66,
            70,
            74,
            78,
            82,
            86,
            90,
            94,
            98,
            102,
            106
        };*/

        IPG_USHORT version;
        IPG_ULONG installedStatus;
        IPG_USHORT modeIndex;
        IPG_FLOAT frequency;
        IPG_FLOAT opticalPowerWatt;
        IPG_FLOAT opticalPowerPerc;
        IPG_ULONG mainStatus;
        IPG_ULONG alarms;
        IPG_ULONG systemAlarms;
        IPG_ULONG extendedAlarms;
        IPG_ULONG warnings;
        IPG_ULONG status;
        IPG_ULONG extendedStatus;
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

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream >> version;
        stream >> installedStatus;
        stream >> modeIndex;
        stream >> frequency;
        stream >> opticalPowerWatt;
        stream >> opticalPowerPerc;
        stream >> mainStatus;
        stream >> alarms;
        stream >> systemAlarms;
        stream >> extendedAlarms;
        stream >> warnings;
        stream >> status;
        stream >> extendedStatus;
        stream >> reserved1;
        stream >> measureFrequency;
        stream >> pulseDuration;
        stream >> measurePower;
        stream >> pulseEnergy;
        stream >> peakPower;
        stream >> analogPowerControl;
        stream >> backReflection;
        stream >> powerSupplyTemp;
        stream >> reserved2;
        stream >> interfaceControllerTemp;
        stream >> reserved3;
        stream >> opticalModule1Temp;
        stream >> opticalModule2Temp;
        stream >> reserved4;

        output->setVersion(version);
        output->setInstalledStatus(installedStatus);
        output->setModeIndex(modeIndex);
        output->setFrequency(frequency);
        output->setOpticalPowerWatt(opticalPowerWatt);
        output->setOpticalPowerPerc(opticalPowerPerc);
        output->setMainStatus(mainStatus);
        output->setAlarms(alarms);
        output->setSystemAlarms(systemAlarms);
        output->setExtendedAlarms(extendedAlarms);
        output->setWarnings(warnings);
        output->setStatus(status);
        output->setExtendedStatus(extendedStatus);
        output->setReserved1(reserved1);
        output->setMeasureFrequency(measureFrequency);
        output->setPulseDuration(pulseDuration);
        output->setMeasurePower(measurePower);
        output->setPulseEnergy(pulseEnergy);
        output->setPeakPower(peakPower);
        output->setAnalogPowerControl(analogPowerControl);
        output->setBackReflection(backReflection);
        output->setPowerSupplyTemp(powerSupplyTemp);
        output->setReserved2(reserved2);
        output->setInterfaceControllerTemp(interfaceControllerTemp);
        output->setReserved3(reserved3);
        output->setOpticalModule1Temp(opticalModule1Temp);
        output->setOpticalModule2Temp(opticalModule2Temp);
        output->setReserved4(reserved4);


        // valorizzazione boolean di allarmi e warning
        output->setInstalledStatusPowerSupply(installedStatus & INSTALLED_STATUS_POWER_SUPPLY);
        output->setInstalledStatusEmission(installedStatus & INSTALLED_STATUS_EMISSION);
        output->setInstalledStatusModulation(installedStatus & INSTALLED_STATUS_MODULATION);
        output->setInstalledStatusGuideLaser(installedStatus & INSTALLED_STATUS_GUIDE_LASER);
        output->setMainStatusAlarms(mainStatus & MAIN_STATUS_ALARMS);
        output->setMainStatusWarnings(mainStatus & MAIN_STATUS_WARNINGS);
        output->setAlarmSystemAlarm(alarms & ALARM_SYSTEM_ALARM);
        output->setAlarmCriticalAlarm(alarms & ALARM_CRITICAL_ALARM);
        output->setAlarmTemperatureAlarm(alarms & ALARM_TEMPERATURE_ALARM);
        output->setAlarmInterfaceController24vAlarm(alarms & ALARM_INTERFACE_CONTROLLER_24V_ALARM);
        output->setAlarmBackReflectionAlarm(alarms & ALARM_BACK_REFLECTION_ALARM);
        output->setAlarmOpticalLinkAlarm(alarms & ALARM_OPTICAL_LINK_ALARM);
        output->setAlarmProcessFiberAlarm(alarms & ALARM_PROCESS_FIBER_ALARM);
        output->setSystemAlarmLaserInterface(systemAlarms & SYSTEM_ALARM_LASER_INTERFACE);
        output->setSystemAlarmOpticalModule1(systemAlarms & SYSTEM_ALARM_OPTICAL_MODULE_1);
        output->setSystemAlarmOpticalModule2(systemAlarms & SYSTEM_ALARM_OPTICAL_MODULE_2);
        output->setSystemAlarmPowerSupply(systemAlarms & SYSTEM_ALARM_POWER_SUPPLY);
        output->setSystemAlarmSafety(systemAlarms & SYSTEM_ALARM_SAFETY);
        output->setSystemAlarmCommunicationOm1(systemAlarms & SYSTEM_ALARM_COMMUNICATION_OM1);
        output->setSystemAlarmCommunicationOm2(systemAlarms & SYSTEM_ALARM_COMMUNICATION_OM2);
        output->setSystemAlarmCommunicationPs(systemAlarms & SYSTEM_ALARM_COMMUNICATION_PS);
        output->setSystemAlarmCommunicationSafety(systemAlarms & SYSTEM_ALARM_COMMUNICATION_SAFETY);
        output->setSystemAlarmFeedingFiberAlarm(extendedAlarms & SYSTEM_ALARM_FEEDING_FIBER_ALARM);
        output->setExtendedAlarmInterfaceVoltageTooLow(extendedAlarms & EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_LOW);
        output->setExtendedAlarmInterfaceVoltageTooHigh(extendedAlarms & EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_HIGH);
        output->setExtendedAlarmInterfaceTemperatureTooLow(extendedAlarms & EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_LOW);
        output->setExtendedAlarmInterfaceTemperatureTooHigh(extendedAlarms & EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_HIGH);
        output->setExtendedAlarmLaserTemperatureTooLow(extendedAlarms & EXTENDED_ALARM_LASER_TEMPERATURE_TOO_LOW);
        output->setExtendedAlarmLaserTemperatureTooHigh(extendedAlarms & EXTENDED_ALARM_LASER_TEMPERATURE_TOO_HIGH);
        output->setExtendedAlarmPsTemperatureTooHigh(extendedAlarms & EXTENDED_ALARM_PS_TEMPERATURE_TOO_HIGH);
        output->setWarningsPrrLow(warnings & WARNINGS_PRR_LOW);
        output->setWarningsPrrHigh(warnings & WARNINGS_PRR_HIGH);
        output->setWarningsSdcardFailure(warnings & WARNINGS_SDCARD_FAILURE);
        output->setWarningsInterfaceTemperatureTooLow(warnings & WARNINGS_INTERFACE_TEMPERATURE_TOO_LOW);
        output->setWarningsInterfaceTemperatureTooHigh(warnings & WARNINGS_INTERFACE_TEMPERATURE_TOO_HIGH);
        output->setWarningsLaserTemperatureTooLow(warnings & WARNINGS_LASER_TEMPERATURE_TOO_LOW);
        output->setWarningsLaserTemperatureTooHigh(warnings & WARNINGS_LASER_TEMPERATURE_TOO_HIGH);
        output->setStatusSafetyCircuitOpen(status & STATUS_SAFETY_CIRCUIT_OPEN);
        output->setStatusPowerSupplyIsOn(status & STATUS_POWER_SUPPLY_IS_ON);
        output->setStatusLaserReady(status & STATUS_LASER_READY);
        output->setStatusEmissionIsOn(status & STATUS_EMISSION_IS_ON);
        output->setStatusGuideLaser(status & STATUS_GUIDE_LASER);
        output->setStatusExternalStop(status & STATUS_EXTERNAL_STOP);
        output->setStatusSafetyCircuitBlocked(status & STATUS_SAFETY_CIRCUIT_BLOCKED);
        output->setStatusHotStop(status & STATUS_HOT_STOP);
        output->setExtendedStatusInterfacePsHotStop(extendedStatus & EXTENDED_STATUS_INTERFACE_PS_HOT_STOP);
        output->setExtendedStatusMainPsHotStop(extendedStatus & EXTENDED_STATUS_MAIN_PS_HOT_STOP);
        output->setExtendedStatusOpticalLinkHotStop(extendedStatus & EXTENDED_STATUS_OPTICAL_LINK_HOT_STOP);
        output->setExtendedStatusSafetyOpenWhileEmissionStop(extendedStatus & EXTENDED_STATUS_SAFETY_OPEN_WHILE_EMISSION_STOP);
        output->setExtendedStatusExternalStopWhileEmission(extendedStatus & EXTENDED_STATUS_EXTERNAL_STOP_WHILE_EMISSION);
        output->setExtendedStatusGlWhileEmissionLow(extendedStatus & EXTENDED_STATUS_GL_WHILE_EMISSION_LOW);


        /*
        bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        installedStatus = bytes.mid(offsets[1], offsets[2] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        modeIndex = bytes.mid(offsets[2], offsets[3] - offsets[2]).toUShort(&ok);
        if (!ok) return false;

        frequency = bytes.mid(offsets[3], offsets[4] - offsets[3]).toFloat(&ok);
        if (!ok) return false;

        opticalPowerWatt = bytes.mid(offsets[4], offsets[5] - offsets[4]).toFloat(&ok);
        if (!ok) return false;

        opticalPowerPerc = bytes.mid(offsets[5], offsets[6] - offsets[5]).toFloat(&ok);
        if (!ok) return false;

        mainStatus = bytes.mid(offsets[6], offsets[7] - offsets[6]).toUInt(&ok);
        if (!ok) return false;

        alarms = bytes.mid(offsets[7], offsets[8] - offsets[7]).toUInt(&ok);
        if (!ok) return false;

        systemAlarms = bytes.mid(offsets[8], offsets[9] - offsets[8]).toUInt(&ok);
        if (!ok) return false;

        extendedAlarms = bytes.mid(offsets[9], offsets[10] - offsets[9]).toUInt(&ok);
        if (!ok) return false;

        warnings = bytes.mid(offsets[10], offsets[11] - offsets[10]).toUInt(&ok);
        if (!ok) return false;

        status = bytes.mid(offsets[11], offsets[12] - offsets[11]).toUInt(&ok);
        if (!ok) return false;

        extendedStatus = bytes.mid(offsets[12], offsets[13] - offsets[12]).toUInt(&ok);
        if (!ok) return false;

        measureFrequency = bytes.mid(offsets[14], offsets[15] - offsets[14]).toFloat(&ok);
        if (!ok) return false;

        pulseDuration = bytes.mid(offsets[15], offsets[16] - offsets[15]).toUInt(&ok);
        if (!ok) return false;

        measurePower = bytes.mid(offsets[16], offsets[17] - offsets[16]).toFloat(&ok);
        if (!ok) return false;

        pulseEnergy = bytes.mid(offsets[17], offsets[18] - offsets[17]).toFloat(&ok);
        if (!ok) return false;

        peakPower = bytes.mid(offsets[18], offsets[19] - offsets[18]).toFloat(&ok);
        if (!ok) return false;

        analogPowerControl = bytes.mid(offsets[19], offsets[20] - offsets[19]).toFloat(&ok);
        if (!ok) return false;

        backReflection = bytes.mid(offsets[20], offsets[21] - offsets[20]).toFloat(&ok);
        if (!ok) return false;

        powerSupplyTemp = bytes.mid(offsets[21], offsets[22] - offsets[21]).toFloat(&ok);
        if (!ok) return false;

        interfaceControllerTemp = bytes.mid(offsets[23], offsets[24] - offsets[23]).toFloat(&ok);
        if (!ok) return false;

        opticalModule1Temp = bytes.mid(offsets[25], offsets[26] - offsets[25]).toFloat(&ok);
        if (!ok) return false;

        opticalModule2Temp = bytes.mid(offsets[26], offsets[27] - offsets[26]).toFloat(&ok);
        if (!ok) return false;

        output->setVersion(version);
        output->setInstalledStatus(installedStatus);
        output->setModeIndex(modeIndex);
        output->setFrequency(frequency);
        output->setOpticalPowerWatt(opticalPowerWatt);
        output->setOpticalPowerPerc(opticalPowerPerc);
        output->setMainStatus(mainStatus);
        output->setAlarms(alarms);
        output->setSystemAlarms(systemAlarms);
        output->setExtendedAlarms(extendedAlarms);
        output->setWarnings(warnings);
        output->setStatus(status);
        output->setExtendedStatus(extendedStatus);
        output->setMeasureFrequency(measureFrequency);
        output->setPulseDuration(pulseDuration);
        output->setMeasurePower(measurePower);
        output->setPulseEnergy(pulseEnergy);
        output->setPeakPower(peakPower);
        output->setAnalogPowerControl(analogPowerControl);
        output->setBackReflection(backReflection);
        output->setPowerSupplyTemp(powerSupplyTemp);
        output->setInterfaceControllerTemp(interfaceControllerTemp);
        output->setOpticalModule1Temp(opticalModule1Temp);
        output->setOpticalModule2Temp(opticalModule2Temp);

        // valorizzazione boolean di allarmi e warning
        output->setAlarmSystemAlarm(alarms & ALARM_SYSTEM_ALARM);
        output->setAlarmCriticalAlarm(alarms & ALARM_CRITICAL_ALARM);
        output->setAlarmTemperatureAlarm(alarms & ALARM_TEMPERATURE_ALARM);
        output->setAlarmInterfaceController24vAlarm(alarms & ALARM_INTERFACE_CONTROLLER_24V_ALARM);
        output->setAlarmBackReflectionAlarm(alarms & ALARM_BACK_REFLECTION_ALARM);
        output->setAlarmOpticalLinkAlarm(alarms & ALARM_OPTICAL_LINK_ALARM);
        output->setAlarmProcessFiberAlarm(alarms & ALARM_PROCESS_FIBER_ALARM);
        output->setSystemAlarmLaserInterface(systemAlarms & SYSTEM_ALARM_LASER_INTERFACE);
        output->setSystemAlarmOpticalModule1(systemAlarms & SYSTEM_ALARM_OPTICAL_MODULE_1);
        output->setSystemAlarmOpticalModule2(systemAlarms & SYSTEM_ALARM_OPTICAL_MODULE_2);
        output->setSystemAlarmPowerSupply(systemAlarms & SYSTEM_ALARM_POWER_SUPPLY);
        output->setSystemAlarmSafety(systemAlarms & SYSTEM_ALARM_SAFETY);
        output->setSystemAlarmCommunicationOm1(systemAlarms & SYSTEM_ALARM_COMMUNICATION_OM1);
        output->setSystemAlarmCommunicationOm2(systemAlarms & SYSTEM_ALARM_COMMUNICATION_OM2);
        output->setSystemAlarmCommunicationPs(systemAlarms & SYSTEM_ALARM_COMMUNICATION_PS);
        output->setSystemAlarmCommunicationSafety(systemAlarms & SYSTEM_ALARM_COMMUNICATION_SAFETY);
        output->setSystemAlarmFeedingFiberAlarm(extendedAlarms & SYSTEM_ALARM_FEEDING_FIBER_ALARM);
        output->setExtendedAlarmInterfaceVoltageTooLow(extendedAlarms & EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_LOW);
        output->setExtendedAlarmInterfaceVoltageTooHigh(extendedAlarms & EXTENDED_ALARM_INTERFACE_VOLTAGE_TOO_HIGH);
        output->setExtendedAlarmInterfaceTemperatureTooLow(extendedAlarms & EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_LOW);
        output->setExtendedAlarmInterfaceTemperatureTooHigh(extendedAlarms & EXTENDED_ALARM_INTERFACE_TEMPERATURE_TOO_HIGH);
        output->setExtendedAlarmLaserTemperatureTooLow(extendedAlarms & EXTENDED_ALARM_LASER_TEMPERATURE_TOO_LOW);
        output->setExtendedAlarmLaserTemperatureTooHigh(extendedAlarms & EXTENDED_ALARM_LASER_TEMPERATURE_TOO_HIGH);
        output->setExtendedAlarmPsTemperatureTooHigh(extendedAlarms & EXTENDED_ALARM_PS_TEMPERATURE_TOO_HIGH);
        output->setWarningsPrrLow(warnings & WARNINGS_PRR_LOW);
        output->setWarningsPrrHigh(warnings & WARNINGS_PRR_HIGH);
        output->setWarningsSdcardFailure(warnings & WARNINGS_SDCARD_FAILURE);
        output->setWarningsInterfaceTemperatureTooLow(warnings & WARNINGS_INTERFACE_TEMPERATURE_TOO_LOW);
        output->setWarningsInterfaceTemperatureTooHigh(warnings & WARNINGS_INTERFACE_TEMPERATURE_TOO_HIGH);
        output->setWarningsLaserTemperatureTooLow(warnings & WARNINGS_LASER_TEMPERATURE_TOO_LOW);
        output->setWarningsLaserTemperatureTooHigh(warnings & WARNINGS_LASER_TEMPERATURE_TOO_HIGH);
        output->setStatusSafetyCircuitOpen(status & STATUS_SAFETY_CIRCUIT_OPEN);
        output->setStatusPowerSupplyIsOn(status & STATUS_POWER_SUPPLY_IS_ON);
        output->setStatusLaserReady(status & STATUS_LASER_READY);
        output->setStatusEmissionIsOn(status & STATUS_EMISSION_IS_ON);
        output->setStatusGuideLaser(status & STATUS_GUIDE_LASER);
        output->setStatusExternalStop(status & STATUS_EXTERNAL_STOP);
        output->setStatusSafetyCircuitBlocked(status & STATUS_SAFETY_CIRCUIT_BLOCKED);
        output->setStatusHotStop(status & STATUS_HOT_STOP);
        output->setExtendedStatusInterfacePsHotStop(extendedStatus & EXTENDED_STATUS_INTERFACE_PS_HOT_STOP);
        output->setExtendedStatusMainPsHotStop(extendedStatus & EXTENDED_STATUS_MAIN_PS_HOT_STOP);
        output->setExtendedStatusOpticalLinkHotStop(extendedStatus & EXTENDED_STATUS_OPTICAL_LINK_HOT_STOP);
        output->setExtendedStatusSafetyOpenWhileEmissionStop(extendedStatus & EXTENDED_STATUS_SAFETY_OPEN_WHILE_EMISSION_STOP);
        output->setExtendedStatusExternalStopWhileEmission(extendedStatus & EXTENDED_STATUS_EXTERNAL_STOP_WHILE_EMISSION);
        output->setExtendedStatusGlWhileEmissionLow(extendedStatus & EXTENDED_STATUS_GL_WHILE_EMISSION_LOW);
        */

        return true;

    }

};

}

#endif // GET_LASER_STATUS_MARSHALLER_HPP
