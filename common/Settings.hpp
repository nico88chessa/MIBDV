#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QSettings>
#include <QString>
#include <QList>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>
#include <data/DigitalInput.hpp>
#include <data/DigitalOutput.hpp>
#include <data/AnalogicInput.hpp>

namespace PROGRAM_NAMESPACE {

class Settings {
public:
    using Ptr = Settings*;
    using ConstPtr = const Settings*;

private:
    Settings();
    ~Settings() { }

    void loadValuesFromFile();
    void writeValuesToFile();

public:
    Settings(const Settings&) = delete;
    void operator=(const Settings&) = delete;

    static Settings& instance();

    int getAxisXStepPerMm() const { return axisXStepPerMm; }
    int getAxisXMinPosMm() const { return axisXMinPosMm; }
    int getAxisXMaxPosMm() const { return axisXMaxPosMm; }
    real getAxisXBaseOffsetMm() const { return axisXBaseOffsetMm; }
    real getAxisXManualSpeedMms() const { return axisXManualSpeedMms; }
    real getAxisXManualAccMms2() const { return axisXManualAccMms2; }
    real getAxisXManualDecMms2() const { return axisXManualDecMms2; }
    real getAxisXOperativeSpeedMms() const { return axisXOperativeSpeedMms; }
    real getAxisXOperativeAccMms2() const { return axisXOperativeAccMms2; }
    real getAxisXOperativeDecMms2() const { return axisXOperativeDecMms2; }
    int getAxisYStepPerMm() const { return axisYStepPerMm; }
    int getAxisYMinPosMm() const { return axisYMinPosMm; }
    int getAxisYMaxPosMm() const { return axisYMaxPosMm; }
    real getAxisYBaseOffsetMm() const { return axisYBaseOffsetMm; }
    real getAxisYManualSpeedMms() const { return axisYManualSpeedMms; }
    real getAxisYManualAccMms2() const { return axisYManualAccMms2; }
    real getAxisYManualDecMms2() const { return axisYManualDecMms2; }
    real getAxisYOperativeSpeedMms() const { return axisYOperativeSpeedMms; }
    real getAxisYOperativeAccMms2() const { return axisYOperativeAccMms2; }
    real getAxisYOperativeDecMms2() const { return axisYOperativeDecMms2; }
    int getAxisZStepPerMm() const { return axisZStepPerMm; }
    int getAxisZMinPosMm() const { return axisZMinPosMm; }
    int getAxisZMaxPosMm() const { return axisZMaxPosMm; }
    real getAxisZBaseOffsetMm() const { return axisZBaseOffsetMm; }
    real getAxisZManualSpeedMms() const { return axisZManualSpeedMms; }
    real getAxisZManualAccMms2() const { return axisZManualAccMms2; }
    real getAxisZManualDecMms2() const { return axisZManualDecMms2; }
    real getAxisZOperativeSpeedMms() const { return axisZOperativeSpeedMms; }
    real getAxisZOperativeAccMms2() const { return axisZOperativeAccMms2; }
    real getAxisZOperativeDecMms2() const { return axisZOperativeDecMms2; }

    QList<DigitalInput> getDigitalInputs() const { return digitalInputs; }
    QList<DigitalOutput> getDigitalOutputs() const { return digitalOutputs; }
    QList<AnalogicInput> getAnalogicInputs() const { return analogicInputs; }

private:
    int axisXStepPerMm;
    int axisXMinPosMm;
    int axisXMaxPosMm;
    real axisXBaseOffsetMm;
    real axisXManualSpeedMms;
    real axisXManualAccMms2;
    real axisXManualDecMms2;
    real axisXOperativeSpeedMms;
    real axisXOperativeAccMms2;
    real axisXOperativeDecMms2;

    int axisYStepPerMm;
    int axisYMinPosMm;
    int axisYMaxPosMm;
    real axisYBaseOffsetMm;
    real axisYManualSpeedMms;
    real axisYManualAccMms2;
    real axisYManualDecMms2;
    real axisYOperativeSpeedMms;
    real axisYOperativeAccMms2;
    real axisYOperativeDecMms2;

    int axisZStepPerMm;
    int axisZMinPosMm;
    int axisZMaxPosMm;
    real axisZBaseOffsetMm;
    real axisZManualSpeedMms;
    real axisZManualAccMms2;
    real axisZManualDecMms2;
    real axisZOperativeSpeedMms;
    real axisZOperativeAccMms2;
    real axisZOperativeDecMms2;

    QList<DigitalInput> digitalInputs;
    QList<DigitalOutput> digitalOutputs;
    QList<AnalogicInput> analogicInputs;

};

}

#endif // SETTINGS_HPP
