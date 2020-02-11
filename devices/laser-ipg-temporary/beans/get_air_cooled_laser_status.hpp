#ifndef GET_AIR_COOLED_LASER_STATUS_HPP
#define GET_AIR_COOLED_LASER_STATUS_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

typedef InputBean GetAirCooledLaserStatusInput;

class GetAirCooledLaserStatusOutput : public OutputBean {
public:
    typedef GetAirCooledLaserStatusOutput* Ptr;
    typedef const GetAirCooledLaserStatusOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_ULONG laserControl;
    IPG_USHORT modeIndex;
    IPG_FLOAT frequency;
    IPG_FLOAT opticalPowerPerc;
    IPG_FLOAT opticalPowerWatt;
    IPG_ULONG mainStatus;
    IPG_ULONG alarms;
    IPG_ULONG reserved;
    IPG_ULONG extendedAlarms;
    IPG_ULONG warnings;
    IPG_ULONG status;
    IPG_ULONG extendedStatus;
    IPG_FLOAT measuredFrequencyKhz;
    IPG_FLOAT pulseDurationns;
    IPG_FLOAT measurePowerWatt;
    IPG_FLOAT pulseEnergy;
    IPG_FLOAT peakPower;
    IPG_FLOAT analogPowerControl;
    IPG_FLOAT backReflection;
    IPG_FLOAT laserTemperature;
    IPG_FLOAT dewPoint;
    IPG_FLOAT humidity;

public:
    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    IPG_ULONG getLaserControl() const { return laserControl; }

    void setLaserControl(const IPG_ULONG& value) { laserControl = value; }

    IPG_USHORT getModeIndex() const { return modeIndex; }

    void setModeIndex(const IPG_USHORT& value) { modeIndex = value; }

    IPG_FLOAT getFrequency() const { return frequency; }

    void setFrequency(const IPG_FLOAT& value) { frequency = value; }

    IPG_FLOAT getOpticalPowerPerc() const { return opticalPowerPerc; }

    void setOpticalPowerPerc(const IPG_FLOAT& value) { opticalPowerPerc = value; }

    IPG_FLOAT getOpticalPowerWatt() const { return opticalPowerWatt; }

    void setOpticalPowerWatt(const IPG_FLOAT& value) { opticalPowerWatt = value; }

    IPG_ULONG getMainStatus() const { return mainStatus; }

    void setMainStatus(const IPG_ULONG& value) { mainStatus = value; }

    IPG_ULONG getAlarms() const { return alarms; }

    void setAlarms(const IPG_ULONG& value) { alarms = value; }

    IPG_ULONG getReserved() const { return reserved; }

    void setReserved(const IPG_ULONG& value) { reserved = value; }

    IPG_ULONG getExtendedAlarms() const { return extendedAlarms; }

    void setExtendedAlarms(const IPG_ULONG& value) { extendedAlarms = value; }

    IPG_ULONG getWarnings() const { return warnings; }

    void setWarnings(const IPG_ULONG& value) { warnings = value; }

    IPG_ULONG getStatus() const { return status; }

    void setStatus(const IPG_ULONG& value) { status = value; }

    IPG_ULONG getExtendedStatus() const { return extendedStatus; }

    void setExtendedStatus(const IPG_ULONG& value) { extendedStatus = value; }

    IPG_FLOAT getMeasuredFrequencyKhz() const { return measuredFrequencyKhz; }

    void setMeasuredFrequencyKhz(const IPG_FLOAT& value) { measuredFrequencyKhz = value; }

    IPG_FLOAT getPulseDurationns() const { return pulseDurationns; }

    void setPulseDurationns(const IPG_FLOAT& value) { pulseDurationns = value; }

    IPG_FLOAT getMeasurePowerWatt() const { return measurePowerWatt; }

    void setMeasurePowerWatt(const IPG_FLOAT& value) { measurePowerWatt = value; }

    IPG_FLOAT getPulseEnergy() const { return pulseEnergy; }

    void setPulseEnergy(const IPG_FLOAT& value) { pulseEnergy = value; }

    IPG_FLOAT getPeakPower() const { return peakPower; }

    void setPeakPower(const IPG_FLOAT& value) { peakPower = value; }

    IPG_FLOAT getAnalogPowerControl() const { return analogPowerControl; }

    void setAnalogPowerControl(const IPG_FLOAT& value) { analogPowerControl = value; }

    IPG_FLOAT getBackReflection() const { return backReflection; }

    void setBackReflection(const IPG_FLOAT& value) { backReflection = value; }

    IPG_FLOAT getLaserTemperature() const { return laserTemperature; }

    void setLaserTemperature(const IPG_FLOAT& value) { laserTemperature = value; }

    IPG_FLOAT getDewPoint() const { return dewPoint; }

    void setDewPoint(const IPG_FLOAT& value) { dewPoint = value; }

    IPG_FLOAT getHumidity() const { return humidity; }

    void setHumidity(const IPG_FLOAT& value) { humidity = value; }


};


}

Q_DECLARE_METATYPE(ipg::GetAirCooledLaserStatusOutput)

#endif // GET_AIR_COOLED_LASER_STATUS_HPP
