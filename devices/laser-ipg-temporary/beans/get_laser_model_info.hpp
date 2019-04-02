#ifndef IPG_LASER_TEMP_GET_LASER_MODEL_INFO_HPP
#define IPG_LASER_TEMP_GET_LASER_MODEL_INFO_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

typedef InputBean GetLaserModelInfoInput;

class GetLaserModelInfoOutput : public OutputBean {
public:
    typedef GetLaserModelInfoOutput* Ptr;
    typedef const GetLaserModelInfoOutput* ConstPtr;

private:
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

    bool installedOptionsHpbInterface;
    bool installedOptionsRs232cInterface;
    bool installedOptionsAllYlpOlrcInterfaces;
    bool installedOptionsApd;
    bool installedOptionsExtendedPrr;
    bool installedOptionsJumpPrr;
    bool installedOptionsBitstream;
    bool installedOptionsAutoLatchPower;
    bool installedOptionsGlInstalled;
    bool installedOptionsHighContrast;
    bool installedOptionsSweepPrr;
    bool installedOptionsGlSafe;
    bool installedOptionsDischargeSafe;

    bool configurableOptionsHpbInterface;
    bool configurableOptionsRs232cInterface;
    bool configurableOptionsAllYlpOlrcInterfaces;
    bool configurableOptionsApd;
    bool configurableOptionsExtendedPrr;
    bool configurableOptionsJumpPrr;
    bool configurableOptionsBitstream;
    bool configurableOptionsAutoLatchPower;
    bool configurableOptionsGlInstalled;
    bool configurableOptionsHighContrast;
    bool configurableOptionsSweepPrr;
    bool configurableOptionsGlSafe;
    bool configurableOptionsDischargeSafe;

    bool laserConfigurationOpticalModule2;
    bool laserConfigurationRemoteUi;
    bool laserConfigurationGl;
    bool laserConfigurationProcessFiberAlarm;
    bool laserConfigurationFeedingFiberAlarm;
    bool laserConfigurationFieldbus;
    bool laserConfigurationPowerSupplyControl;
    bool laserConfigurationAnalogPowerControl;
    bool laserConfigurationSafetyControl;


public:
    IPG_FLOAT getMinimumTemperature() const { return minimumTemperature; }

    void setMinimumTemperature(const IPG_FLOAT& value) { minimumTemperature = value; }

    IPG_FLOAT getMaximumFrequency() const { return maximumFrequency; }

    void setMaximumFrequency(const IPG_FLOAT& value) { maximumFrequency = value; }

    IPG_FLOAT getMinimumFrequency() const { return minimumFrequency; }

    void setMinimumFrequency(const IPG_FLOAT& value) { minimumFrequency = value; }

    IPG_FLOAT getNominalPeakPower() const { return nominalPeakPower; }

    void setNominalPeakPower(const IPG_FLOAT& value) { nominalPeakPower = value; }

    IPG_FLOAT getNominalAveragePower() const { return nominalAveragePower; }

    void setNominalAveragePower(const IPG_FLOAT& value) { nominalAveragePower = value; }

    IPG_FLOAT getNominalPulseDuration() const { return nominalPulseDuration; }

    void setNominalPulseDuration(const IPG_FLOAT& value) { nominalPulseDuration = value; }

    IPG_FLOAT getNominalPulseEnergy() const { return nominalPulseEnergy; }

    void setNominalPulseEnergy(const IPG_FLOAT& value) { nominalPulseEnergy = value; }

    IPG_FLOAT getNominalFrequency() const { return nominalFrequency; }

    void setNominalFrequency(const IPG_FLOAT& value) { nominalFrequency = value; }

    IPG_USHORT getInstalledModes() const { return installedModes; }

    void setInstalledModes(const IPG_USHORT& value) { installedModes = value; }

    IPG_ULONG getLaserConfiguration() const { return laserConfiguration; }

    void setLaserConfiguration(const IPG_ULONG& value) { laserConfiguration = value; }

    IPG_ULONG getConfigurableOptions() const { return configurableOptions; }

    void setConfigurableOptions(const IPG_ULONG& value) { configurableOptions = value; }

    IPG_ULONG getInstalledOptions() const { return installedOptions; }

    void setInstalledOptions(const IPG_ULONG& value) { installedOptions = value; }

    QString getSerialNumberLaser() const { return serialNumberLaser; }

    void setSerialNumberLaser(const QString& value) { serialNumberLaser = value; }

    QString getModelLaser() const { return modelLaser; }

    void setModelLaser(const QString& value) { modelLaser = value; }

    QString getSerialNumberUI() const { return serialNumberUI; }

    void setSerialNumberUI(const QString& value) { serialNumberUI = value; }

    QString getModelUI() const { return modelUI; }

    void setModelUI(const QString& value) { modelUI = value; }

    QString getManufacturer() const { return manufacturer; }

    void setManufacturer(const QString& value) { manufacturer = value; }

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    IPG_FLOAT getMaximumTemperature() const { return maximumTemperature; }

    void setMaximumTemperature(const IPG_FLOAT& value) { maximumTemperature = value; }

    // valori boolean

    bool getInstalledOptionsRs232cInterface() const { return installedOptionsRs232cInterface; }

    void setInstalledOptionsRs232cInterface(bool value) { installedOptionsRs232cInterface = value; }

    bool getInstalledOptionsAllYlpOlrcInterfaces() const { return installedOptionsAllYlpOlrcInterfaces; }

    void setInstalledOptionsAllYlpOlrcInterfaces(bool value) { installedOptionsAllYlpOlrcInterfaces = value; }

    bool getInstalledOptionsApd() const { return installedOptionsApd; }

    void setInstalledOptionsApd(bool value) { installedOptionsApd = value; }

    bool getInstalledOptionsExtendedPrr() const { return installedOptionsExtendedPrr; }

    void setInstalledOptionsExtendedPrr(bool value) { installedOptionsExtendedPrr = value; }

    bool getInstalledOptionsJumpPrr() const { return installedOptionsJumpPrr; }

    void setInstalledOptionsJumpPrr(bool value) { installedOptionsJumpPrr = value; }

    bool getInstalledOptionsBitstream() const { return installedOptionsBitstream; }

    void setInstalledOptionsBitstream(bool value) { installedOptionsBitstream = value; }

    bool getInstalledOptionsAutoLatchPower() const { return installedOptionsAutoLatchPower; }

    void setInstalledOptionsAutoLatchPower(bool value) { installedOptionsAutoLatchPower = value; }

    bool getInstalledOptionsGlInstalled() const { return installedOptionsGlInstalled; }

    void setInstalledOptionsGlInstalled(bool value) { installedOptionsGlInstalled = value; }

    bool getInstalledOptionsHighContrast() const { return installedOptionsHighContrast; }

    void setInstalledOptionsHighContrast(bool value) { installedOptionsHighContrast = value; }

    bool getInstalledOptionsSweepPrr() const { return installedOptionsSweepPrr; }

    void setInstalledOptionsSweepPrr(bool value) { installedOptionsSweepPrr = value; }

    bool getInstalledOptionsGlSafe() const { return installedOptionsGlSafe; }

    void setInstalledOptionsGlSafe(bool value) { installedOptionsGlSafe = value; }

    bool getInstalledOptionsDischargeSafe() const { return installedOptionsDischargeSafe; }

    void setInstalledOptionsDischargeSafe(bool value) { installedOptionsDischargeSafe = value; }

    bool getConfigurableOptionsHpbInterface() const { return configurableOptionsHpbInterface; }

    void setConfigurableOptionsHpbInterface(bool value) { configurableOptionsHpbInterface = value; }

    bool getConfigurableOptionsRs232cInterface() const { return configurableOptionsRs232cInterface; }

    void setConfigurableOptionsRs232cInterface(bool value) { configurableOptionsRs232cInterface = value; }

    bool getConfigurableOptionsAllYlpOlrcInterfaces() const { return configurableOptionsAllYlpOlrcInterfaces; }

    void setConfigurableOptionsAllYlpOlrcInterfaces(bool value) { configurableOptionsAllYlpOlrcInterfaces = value; }

    bool getConfigurableOptionsApd() const { return configurableOptionsApd; }

    void setConfigurableOptionsApd(bool value) { configurableOptionsApd = value; }

    bool getConfigurableOptionsExtendedPrr() const { return configurableOptionsExtendedPrr; }

    void setConfigurableOptionsExtendedPrr(bool value) { configurableOptionsExtendedPrr = value; }

    bool getConfigurableOptionsJumpPrr() const { return configurableOptionsJumpPrr; }

    void setConfigurableOptionsJumpPrr(bool value) { configurableOptionsJumpPrr = value; }

    bool getConfigurableOptionsBitstream() const { return configurableOptionsBitstream; }

    void setConfigurableOptionsBitstream(bool value) { configurableOptionsBitstream = value; }

    bool getConfigurableOptionsAutoLatchPower() const { return configurableOptionsAutoLatchPower; }

    void setConfigurableOptionsAutoLatchPower(bool value) { configurableOptionsAutoLatchPower = value; }

    bool getConfigurableOptionsGlInstalled() const { return configurableOptionsGlInstalled; }

    void setConfigurableOptionsGlInstalled(bool value) { configurableOptionsGlInstalled = value; }

    bool getConfigurableOptionsHighContrast() const { return configurableOptionsHighContrast; }

    void setConfigurableOptionsHighContrast(bool value) { configurableOptionsHighContrast = value; }

    bool getConfigurableOptionsSweepPrr() const { return configurableOptionsSweepPrr; }

    void setConfigurableOptionsSweepPrr(bool value) { configurableOptionsSweepPrr = value; }

    bool getConfigurableOptionsGlSafe() const { return configurableOptionsGlSafe; }

    void setConfigurableOptionsGlSafe(bool value) { configurableOptionsGlSafe = value; }

    bool getConfigurableOptionsDischargeSafe() const { return configurableOptionsDischargeSafe; }

    void setConfigurableOptionsDischargeSafe(bool value) { configurableOptionsDischargeSafe = value; }

    bool getLaserConfigurationOpticalModule2() const { return laserConfigurationOpticalModule2; }

    void setLaserConfigurationOpticalModule2(bool value) { laserConfigurationOpticalModule2 = value; }

    bool getLaserConfigurationRemoteUi() const { return laserConfigurationRemoteUi; }

    void setLaserConfigurationRemoteUi(bool value) { laserConfigurationRemoteUi = value; }

    bool getLaserConfigurationGl() const { return laserConfigurationGl; }

    void setLaserConfigurationGl(bool value) { laserConfigurationGl = value; }

    bool getLaserConfigurationProcessFiberAlarm() const { return laserConfigurationProcessFiberAlarm; }

    void setLaserConfigurationProcessFiberAlarm(bool value) { laserConfigurationProcessFiberAlarm = value; }

    bool getLaserConfigurationFeedingFiberAlarm() const { return laserConfigurationFeedingFiberAlarm; }

    void setLaserConfigurationFeedingFiberAlarm(bool value) { laserConfigurationFeedingFiberAlarm = value; }

    bool getLaserConfigurationFieldbus() const { return laserConfigurationFieldbus; }

    void setLaserConfigurationFieldbus(bool value) { laserConfigurationFieldbus = value; }

    bool getLaserConfigurationPowerSupplyControl() const { return laserConfigurationPowerSupplyControl; }

    void setLaserConfigurationPowerSupplyControl(bool value) { laserConfigurationPowerSupplyControl = value; }

    bool getLaserConfigurationAnalogPowerControl() const { return laserConfigurationAnalogPowerControl; }

    void setLaserConfigurationAnalogPowerControl(bool value) { laserConfigurationAnalogPowerControl = value; }

    bool getLaserConfigurationSafetyControl() const { return laserConfigurationSafetyControl; }

    void setLaserConfigurationSafetyControl(bool value) { laserConfigurationSafetyControl = value; }

    bool getInstalledOptionsHpbInterface() const { return installedOptionsHpbInterface; }

    void setInstalledOptionsHpbInterface(bool value) { installedOptionsHpbInterface = value; }

};


}

Q_DECLARE_METATYPE(ipg::GetLaserModelInfoOutput)

#endif // GET_LASER_MODEL_INFO_HPP
