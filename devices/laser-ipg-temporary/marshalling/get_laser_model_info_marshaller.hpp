#ifndef IPG_LASER_TEMP_GET_LASER_MODEL_INFO_MARSHALLER_HPP
#define IPG_LASER_TEMP_GET_LASER_MODEL_INFO_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/get_laser_model_info.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class GetLaserModelInfoMarshaller : public IpgMarshaller<GetLaserModelInfoInput, GetLaserModelInfoOutput> {
public:
    typedef GetLaserModelInfoMarshaller* Ptr;
    typedef const GetLaserModelInfoMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetLaserModelInfoOutput::Ptr output) {

        const int offsets[20] = {
            0,
            2,
            66,
            194,
            258,
            386,
            450,
            454,
            458,
            462,
            464,
            468,
            472,
            476,
            480,
            484,
            488,
            492,
            496,
            500
        };

        IPG_USHORT version;
        QString manufacturer;
        QString modelUI;
        QString serialNumberUI;
        QString modelLaser;
        QString serialNumberLaser;
        IPG_ULONG installedOptions;
        IPG_ULONG configurableOptions;
        IPG_ULONG laserConfiguration;
        IPG_USHORT installedModes;
        IPG_FLOAT nominalFrequency;
        IPG_FLOAT nominalPulseEnergy;
        IPG_FLOAT nominalPulseDuration;
        IPG_FLOAT nominalAveragePower;
        IPG_FLOAT nominalPeakPower;
        IPG_FLOAT minimumFrequency;
        IPG_FLOAT maximumFrequency;
        IPG_FLOAT minimumTemperature;
        IPG_FLOAT maximumTemperature;
        int size;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream >> version;

        size = offsets[2] - offsets[1];
        char* manufacturerBuffer = new char[size];
        stream.readRawData(&manufacturerBuffer[0], size);
        manufacturer.append(manufacturerBuffer);
        delete[] manufacturerBuffer;

        size = offsets[3] - offsets[2];
        char* modelUIBuffer = new char[size];
        stream.readRawData(&modelUIBuffer[0], size);
        modelUI.append(modelUIBuffer);
        delete[] modelUIBuffer;

        size = offsets[4] - offsets[3];
        char* serialNumberUIBuffer = new char[size];
        stream.readRawData(&serialNumberUIBuffer[0], size);
        serialNumberUI.append(serialNumberUIBuffer);
        delete[] serialNumberUIBuffer;

        size = offsets[5] - offsets[4];
        char* modelLaserBuffer = new char[size];
        stream.readRawData(&modelLaserBuffer[0], size);
        modelLaser.append(modelLaserBuffer);
        delete[] modelLaserBuffer;

        size = offsets[6] - offsets[5];
        char* serialNumberLaserBuffer = new char[size];
        stream.readRawData(&serialNumberLaserBuffer[0], size);
        serialNumberLaser.append(serialNumberLaserBuffer);
        delete[] serialNumberLaserBuffer;

        stream >> installedOptions;
        stream >> configurableOptions;
        stream >> laserConfiguration;
        stream >> installedModes;
        stream >> nominalFrequency;
        stream >> nominalPulseEnergy;
        stream >> nominalPulseDuration;
        stream >> nominalAveragePower;
        stream >> nominalPeakPower;
        stream >> minimumFrequency;
        stream >> maximumFrequency;
        stream >> minimumTemperature;
        stream >> maximumTemperature;

        output->setVersion(version);
        output->setManufacturer(manufacturer);
        output->setModelUI(modelUI);
        output->setSerialNumberUI(serialNumberUI);
        output->setModelLaser(modelLaser);
        output->setSerialNumberLaser(serialNumberLaser);
        output->setInstalledOptions(installedOptions);
        output->setConfigurableOptions(configurableOptions);
        output->setLaserConfiguration(laserConfiguration);
        output->setInstalledModes(installedModes);
        output->setNominalFrequency(nominalFrequency);
        output->setNominalPulseEnergy(nominalPulseEnergy);
        output->setNominalPulseDuration(nominalPulseDuration);
        output->setNominalAveragePower(nominalAveragePower);
        output->setNominalPeakPower(nominalPeakPower);
        output->setMinimumFrequency(minimumFrequency);
        output->setMaximumFrequency(maximumFrequency);
        output->setMinimumTemperature(minimumTemperature);
        output->setMaximumTemperature(maximumTemperature);

        // valorizzazione valori boolean

        output->setInstalledOptionsHpbInterface(installedOptions & LASER_OPTIONS_HPB_INTERFACE);
        output->setInstalledOptionsRs232cInterface(installedOptions & LASER_OPTIONS_RS232C_INTERFACE);
        output->setInstalledOptionsAllYlpOlrcInterfaces(installedOptions & LASER_OPTIONS_ALL_YLP_OLRC_INTERFACES);
        output->setInstalledOptionsApd(installedOptions & LASER_OPTIONS_APD);
        output->setInstalledOptionsExtendedPrr(installedOptions & LASER_OPTIONS_EXTENDED_PRR);
        output->setInstalledOptionsJumpPrr(installedOptions & LASER_OPTIONS_JUMP_PRR);
        output->setInstalledOptionsBitstream(installedOptions & LASER_OPTIONS_BITSTREAM);
        output->setInstalledOptionsAutoLatchPower(installedOptions & LASER_OPTIONS_AUTO_LATCH_POWER);
        output->setInstalledOptionsGlInstalled(installedOptions & LASER_OPTIONS_GL_INSTALLED);
        output->setInstalledOptionsHighContrast(installedOptions & LASER_OPTIONS_HIGH_CONTRAST);
        output->setInstalledOptionsSweepPrr(installedOptions & LASER_OPTIONS_SWEEP_PRR);
        output->setInstalledOptionsGlSafe(installedOptions & LASER_OPTIONS_GL_SAFE);
        output->setInstalledOptionsDischargeSafe(installedOptions & LASER_OPTIONS_DISCHARGE_SAFE);

        output->setConfigurableOptionsHpbInterface(configurableOptions & LASER_OPTIONS_HPB_INTERFACE);
        output->setConfigurableOptionsRs232cInterface(configurableOptions & LASER_OPTIONS_RS232C_INTERFACE);
        output->setConfigurableOptionsAllYlpOlrcInterfaces(configurableOptions & LASER_OPTIONS_ALL_YLP_OLRC_INTERFACES);
        output->setConfigurableOptionsApd(configurableOptions & LASER_OPTIONS_APD);
        output->setConfigurableOptionsExtendedPrr(configurableOptions & LASER_OPTIONS_EXTENDED_PRR);
        output->setConfigurableOptionsJumpPrr(configurableOptions & LASER_OPTIONS_JUMP_PRR);
        output->setConfigurableOptionsBitstream(configurableOptions & LASER_OPTIONS_BITSTREAM);
        output->setConfigurableOptionsAutoLatchPower(configurableOptions & LASER_OPTIONS_AUTO_LATCH_POWER);
        output->setConfigurableOptionsGlInstalled(configurableOptions & LASER_OPTIONS_GL_INSTALLED);
        output->setConfigurableOptionsHighContrast(configurableOptions & LASER_OPTIONS_HIGH_CONTRAST);
        output->setConfigurableOptionsSweepPrr(configurableOptions & LASER_OPTIONS_SWEEP_PRR);
        output->setConfigurableOptionsGlSafe(configurableOptions & LASER_OPTIONS_GL_SAFE);
        output->setConfigurableOptionsDischargeSafe(configurableOptions & LASER_OPTIONS_DISCHARGE_SAFE);

        output->setLaserConfigurationOpticalModule2(laserConfiguration & LASER_CONFIGURATION_OPTICAL_MODULE_2);
        output->setLaserConfigurationRemoteUi(laserConfiguration & LASER_CONFIGURATION_REMOTE_UI);
        output->setLaserConfigurationGl(laserConfiguration & LASER_CONFIGURATION_GL);
        output->setLaserConfigurationProcessFiberAlarm(laserConfiguration & LASER_CONFIGURATION_PROCESS_FIBER_ALARM);
        output->setLaserConfigurationFeedingFiberAlarm(laserConfiguration & LASER_CONFIGURATION_FEEDING_FIBER_ALARM);
        output->setLaserConfigurationFieldbus(laserConfiguration & LASER_CONFIGURATION_FIELDBUS);
        output->setLaserConfigurationPowerSupplyControl(laserConfiguration & LASER_CONFIGURATION_POWER_SUPPLY_CONTROL);
        output->setLaserConfigurationAnalogPowerControl(laserConfiguration & LASER_CONFIGURATION_ANALOG_POWER_CONTROL);
        output->setLaserConfigurationSafetyControl(laserConfiguration & LASER_CONFIGURATION_SAFETY_CONTROL);


        /*****************************************************
        bool ok;
        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        manufacturer < bytes.mid(offsets[1], offsets[2] - offsets[1]);
        modelUI < bytes.mid(offsets[2], offsets[3] - offsets[2]);
        serialNumberUI < bytes.mid(offsets[3], offsets[4] - offsets[3]);
        modelLaser < bytes.mid(offsets[4], offsets[5] - offsets[4]);
        serialNumberLaser < bytes.mid(offsets[5], offsets[6] - offsets[5]);

        installedOptions = bytes.mid(offsets[6], offsets[7] - offsets[6]).toULong(&ok);
        if (!ok) return false;

        configurableOptions = bytes.mid(offsets[7], offsets[8] - offsets[7]).toULong(&ok);
        if (!ok) return false;

        laserConfiguration = bytes.mid(offsets[8], offsets[9] - offsets[8]).toULong(&ok);
        if (!ok) return false;

        installedModes = bytes.mid(offsets[9], offsets[10] - offsets[9]).toUShort(&ok);
        if (!ok) return false;

        nominalFrequency = bytes.mid(offsets[10], offsets[11] - offsets[10]).toFloat(&ok);
        if (!ok) return false;

        nominalPulseEnergy = bytes.mid(offsets[11], offsets[12] - offsets[11]).toFloat(&ok);
        if (!ok) return false;

        nominalPulseDuration = bytes.mid(offsets[12], offsets[13] - offsets[12]).toFloat(&ok);
        if (!ok) return false;

        nominalAveragePower = bytes.mid(offsets[13], offsets[14] - offsets[13]).toFloat(&ok);
        if (!ok) return false;

        nominalPeakPower = bytes.mid(offsets[14], offsets[15] - offsets[14]).toFloat(&ok);
        if (!ok) return false;

        minimumFrequency = bytes.mid(offsets[15], offsets[16] - offsets[15]).toFloat(&ok);
        if (!ok) return false;

        maximumFrequency = bytes.mid(offsets[16], offsets[17] - offsets[16]).toFloat(&ok);
        if (!ok) return false;

        minimumTemperature = bytes.mid(offsets[17], offsets[18] - offsets[17]).toFloat(&ok);
        if (!ok) return false;

        maximumTemperature = bytes.mid(offsets[18], offsets[19] - offsets[18]).toFloat(&ok);
        if (!ok) return false;

        output->setVersion(version);
        output->setManufacturer(manufacturer);
        output->setModelUI(modelUI);
        output->setSerialNumberUI(serialNumberUI);
        output->setModelLaser(modelLaser);
        output->setSerialNumberLaser(serialNumberLaser);
        output->setInstalledOptions(installedOptions);
        output->setConfigurableOptions(configurableOptions);
        output->setLaserConfiguration(laserConfiguration);
        output->setInstalledModes(installedModes);
        output->setNominalFrequency(nominalFrequency);
        output->setNominalPulseEnergy(nominalPulseEnergy);
        output->setNominalPulseDuration(nominalPulseDuration);
        output->setNominalAveragePower(nominalAveragePower);
        output->setNominalPeakPower(nominalPeakPower);
        output->setMinimumFrequency(minimumFrequency);
        output->setMaximumFrequency(maximumFrequency);
        output->setMinimumTemperature(minimumTemperature);
        output->setMaximumTemperature(maximumTemperature);

        // valorizzazione valori boolean

        output->setInstalledOptionsHpbInterface(installedOptions & LASER_OPTIONS_HPB_INTERFACE);
        output->setInstalledOptionsRs232cInterface(installedOptions & LASER_OPTIONS_RS232C_INTERFACE);
        output->setInstalledOptionsAllYlpOlrcInterfaces(installedOptions & LASER_OPTIONS_ALL_YLP_OLRC_INTERFACES);
        output->setInstalledOptionsApd(installedOptions & LASER_OPTIONS_APD);
        output->setInstalledOptionsExtendedPrr(installedOptions & LASER_OPTIONS_EXTENDED_PRR);
        output->setInstalledOptionsJumpPrr(installedOptions & LASER_OPTIONS_JUMP_PRR);
        output->setInstalledOptionsBitstream(installedOptions & LASER_OPTIONS_BITSTREAM);
        output->setInstalledOptionsAutoLatchPower(installedOptions & LASER_OPTIONS_AUTO_LATCH_POWER);
        output->setInstalledOptionsGlInstalled(installedOptions & LASER_OPTIONS_GL_INSTALLED);
        output->setInstalledOptionsHighContrast(installedOptions & LASER_OPTIONS_HIGH_CONTRAST);
        output->setInstalledOptionsSweepPrr(installedOptions & LASER_OPTIONS_SWEEP_PRR);
        output->setInstalledOptionsGlSafe(installedOptions & LASER_OPTIONS_GL_SAFE);
        output->setInstalledOptionsDischargeSafe(installedOptions & LASER_OPTIONS_DISCHARGE_SAFE);

        output->setConfigurableOptionsHpbInterface(configurableOptions & LASER_OPTIONS_HPB_INTERFACE);
        output->setConfigurableOptionsRs232cInterface(configurableOptions & LASER_OPTIONS_RS232C_INTERFACE);
        output->setConfigurableOptionsAllYlpOlrcInterfaces(configurableOptions & LASER_OPTIONS_ALL_YLP_OLRC_INTERFACES);
        output->setConfigurableOptionsApd(configurableOptions & LASER_OPTIONS_APD);
        output->setConfigurableOptionsExtendedPrr(configurableOptions & LASER_OPTIONS_EXTENDED_PRR);
        output->setConfigurableOptionsJumpPrr(configurableOptions & LASER_OPTIONS_JUMP_PRR);
        output->setConfigurableOptionsBitstream(configurableOptions & LASER_OPTIONS_BITSTREAM);
        output->setConfigurableOptionsAutoLatchPower(configurableOptions & LASER_OPTIONS_AUTO_LATCH_POWER);
        output->setConfigurableOptionsGlInstalled(configurableOptions & LASER_OPTIONS_GL_INSTALLED);
        output->setConfigurableOptionsHighContrast(configurableOptions & LASER_OPTIONS_HIGH_CONTRAST);
        output->setConfigurableOptionsSweepPrr(configurableOptions & LASER_OPTIONS_SWEEP_PRR);
        output->setConfigurableOptionsGlSafe(configurableOptions & LASER_OPTIONS_GL_SAFE);
        output->setConfigurableOptionsDischargeSafe(configurableOptions & LASER_OPTIONS_DISCHARGE_SAFE);

        output->setLaserConfigurationOpticalModule2(laserConfiguration & LASER_CONFIGURATION_OPTICAL_MODULE_2);
        output->setLaserConfigurationRemoteUi(laserConfiguration & LASER_CONFIGURATION_REMOTE_UI);
        output->setLaserConfigurationGl(laserConfiguration & LASER_CONFIGURATION_GL);
        output->setLaserConfigurationProcessFiberAlarm(laserConfiguration & LASER_CONFIGURATION_PROCESS_FIBER_ALARM);
        output->setLaserConfigurationFeedingFiberAlarm(laserConfiguration & LASER_CONFIGURATION_FEEDING_FIBER_ALARM);
        output->setLaserConfigurationFieldbus(laserConfiguration & LASER_CONFIGURATION_FIELDBUS);
        output->setLaserConfigurationPowerSupplyControl(laserConfiguration & LASER_CONFIGURATION_POWER_SUPPLY_CONTROL);
        output->setLaserConfigurationAnalogPowerControl(laserConfiguration & LASER_CONFIGURATION_ANALOG_POWER_CONTROL);
        output->setLaserConfigurationSafetyControl(laserConfiguration & LASER_CONFIGURATION_SAFETY_CONTROL);
        ****************************************************/
        return true;

    }

};

}

#endif // GET_LASER_MODEL_INFO_MARSHALLER_HPP
