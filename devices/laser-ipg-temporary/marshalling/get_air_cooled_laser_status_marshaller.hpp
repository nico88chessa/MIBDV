#ifndef GETAIRCOOLEDLASERSTATUSMARSHALLER_HPP
#define GETAIRCOOLEDLASERSTATUSMARSHALLER_HPP


#include <laser-ipg-temporary/beans/get_air_cooled_laser_status.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

/*
 * NOTE NIC 11/02/2020
 * classe fatta in velocita'. Da rivedere il mapping
 * soprattutto per la parte dei FLAG
 */

namespace ipg {

class GetAirCooledLaserStatusMarshaller : public IpgMarshaller<GetAirCooledLaserStatusInput, GetAirCooledLaserStatusOutput> {
public:
    typedef GetAirCooledLaserStatusMarshaller* Ptr;
    typedef const GetAirCooledLaserStatusMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetAirCooledLaserStatusOutput::Ptr output) {

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

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream >> version;
        stream >> laserControl;
        stream >> modeIndex;
        stream >> frequency;
        stream >> opticalPowerPerc;
        stream >> opticalPowerWatt;
        stream >> mainStatus;
        stream >> alarms;
        stream >> reserved;
        stream >> extendedAlarms;
        stream >> warnings;
        stream >> status;
        stream >> extendedStatus;
        stream >> measuredFrequencyKhz;
        stream >> pulseDurationns;
        stream >> measurePowerWatt;
        stream >> pulseEnergy;
        stream >> peakPower;
        stream >> analogPowerControl;
        stream >> backReflection;
        stream >> laserTemperature;
        stream >> dewPoint;
        stream >> humidity;

        output->setVersion(version);
        output->setLaserControl(laserControl);
        output->setModeIndex(modeIndex);
        output->setFrequency(frequency);
        output->setOpticalPowerPerc(opticalPowerPerc);
        output->setOpticalPowerWatt(opticalPowerWatt);
        output->setMainStatus(mainStatus);
        output->setAlarms(alarms);
        output->setReserved(reserved);
        output->setExtendedAlarms(extendedAlarms);
        output->setWarnings(warnings);
        output->setStatus(status);
        output->setExtendedStatus(extendedStatus);
        output->setMeasuredFrequencyKhz(measuredFrequencyKhz);
        output->setPulseDurationns(pulseDurationns);
        output->setMeasurePowerWatt(measurePowerWatt);
        output->setPulseEnergy(pulseEnergy);
        output->setPeakPower(peakPower);
        output->setAnalogPowerControl(analogPowerControl);
        output->setBackReflection(backReflection);
        output->setLaserTemperature(laserTemperature);
        output->setDewPoint(dewPoint);
        output->setHumidity(humidity);

        return true;

    }

};

}

#endif // GETAIRCOOLEDSTATUSMARSHALLER_HPP
