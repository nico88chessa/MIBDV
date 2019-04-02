#ifndef IPG_LASER_TEMP_GET_MODE_PARAMETER_HPP
#define IPG_LASER_TEMP_GET_MODE_PARAMETER_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class GetModeParameterInput : public InputBean {
public:
    typedef GetModeParameterInput* Ptr;
    typedef const GetModeParameterInput* ConstPtr;

private:
    IPG_USHORT modeIndex;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 2;

public:
    GetModeParameterInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getModeIndex() const { return modeIndex; }

    void setModeIndex(const IPG_USHORT& value) { modeIndex = value; }

};


class GetModeParameterOutput : public OutputBean {
public:
    typedef GetModeParameterOutput* Ptr;
    typedef const GetModeParameterOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_USHORT modeIndex;
    IPG_FLOAT nominalFrequency;
    IPG_FLOAT nominalPulseEnergy;
    IPG_FLOAT nominalPulseDuration;
    IPG_FLOAT nominalAveragePower;
    IPG_FLOAT nominalPeakPower;
    IPG_FLOAT minimumFrequency;
    IPG_FLOAT maximumFrequency;
    QString modeDescription;

public:
    IPG_USHORT getModeIndex() const { return modeIndex; }

    void setModeIndex(const IPG_USHORT& value) { modeIndex = value; }

    IPG_FLOAT getNominalFrequency() const { return nominalFrequency; }

    void setNominalFrequency(const IPG_FLOAT& value) { nominalFrequency = value; }

    IPG_FLOAT getNominalPulseEnergy() const { return nominalPulseEnergy; }

    void setNominalPulseEnergy(const IPG_FLOAT& value) { nominalPulseEnergy = value; }

    IPG_FLOAT getNominalPulseDuration() const { return nominalPulseDuration; }

    void setNominalPulseDuration(const IPG_FLOAT& value) { nominalPulseDuration = value; }

    IPG_FLOAT getNominalAveragePower() const { return nominalAveragePower; }

    void setNominalAveragePower(const IPG_FLOAT& value) { nominalAveragePower = value; }

    IPG_FLOAT getNominalPeakPower() const { return nominalPeakPower; }

    void setNominalPeakPower(const IPG_FLOAT& value) { nominalPeakPower = value; }

    IPG_FLOAT getMinimumFrequency() const { return minimumFrequency; }

    void setMinimumFrequency(const IPG_FLOAT& value) { minimumFrequency = value; }

    IPG_FLOAT getMaximumFrequency() const { return maximumFrequency; }

    void setMaximumFrequency(const IPG_FLOAT& value) { maximumFrequency = value; }

    QString getModeDescription() const { return modeDescription; }

    void setModeDescription(const QString& value) { modeDescription = value; }

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }
};


}

Q_DECLARE_METATYPE(ipg::GetModeParameterInput)
Q_DECLARE_METATYPE(ipg::GetModeParameterOutput)

#endif // GET_MODE_PARAMETER_HPP
