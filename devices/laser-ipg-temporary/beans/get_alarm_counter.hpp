#ifndef IPG_LASER_TEMP_GET_ALARM_COUNTER_HPP
#define IPG_LASER_TEMP_GET_ALARM_COUNTER_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

typedef InputBean GetAlarmCounterInput;

class GetAlarmCounterOutput : public OutputBean {
public:
    typedef GetAlarmCounterOutput* Ptr;
    typedef const GetAlarmCounterOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_USHORT systemAlarm;
    IPG_USHORT laserInterface;
    IPG_USHORT opticalModule1;
    IPG_USHORT opticalModule2;
    IPG_USHORT powerSupply;
    IPG_USHORT safety;
    IPG_USHORT communicationOM1;
    IPG_USHORT communicationOM2;
    IPG_USHORT communicationPS;
    IPG_USHORT communicationSafety;
    IPG_USHORT criticalAlarm;
    IPG_USHORT temperatureAlarm;
    IPG_USHORT interfaceTemperatureTooLow;
    IPG_USHORT interfaceTemperatureTooHigh;
    IPG_USHORT laserTemperatureTooLow;
    IPG_USHORT laserTemperatureTooHigh;
    IPG_USHORT reserved1;
    IPG_USHORT reserved2;
    IPG_USHORT psTemperatureTooHigh;
    IPG_USHORT reserved3;
    IPG_USHORT interfaceController24VAlarm;
    IPG_USHORT interfaceVoltageTooLow;
    IPG_USHORT interfaceVoltageTooHigh;
    IPG_USHORT backReflectionAlarm;
    IPG_USHORT opticalLinkAlarm;
    IPG_USHORT processFiberAlarm;
    IPG_USHORT FeedingFiberAlarm;
    IPG_USHORT hotStop;
    IPG_USHORT interfacePSHotStop;
    IPG_USHORT mainPSHotStop;
    IPG_USHORT opticalLinkHotStop;
    IPG_USHORT safetyOpenWhileEmission;
    IPG_USHORT externalStopWhileEmission;
    IPG_USHORT GLWhileEmission;
    IPG_USHORT reserved4;
    IPG_USHORT reserved5;

public:
    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    IPG_USHORT getPsTemperatureTooHigh() const { return psTemperatureTooHigh; }

    void setPsTemperatureTooHigh(const IPG_USHORT& value) { psTemperatureTooHigh = value; }

    IPG_USHORT getInterfaceController24VAlarm() const { return interfaceController24VAlarm; }

    void setInterfaceController24VAlarm(const IPG_USHORT& value) { interfaceController24VAlarm = value; }

    IPG_USHORT getInterfaceVoltageTooLow() const { return interfaceVoltageTooLow; }

    void setInterfaceVoltageTooLow(const IPG_USHORT& value) { interfaceVoltageTooLow = value; }

    IPG_USHORT getInterfaceVoltageTooHigh() const { return interfaceVoltageTooHigh; }

    void setInterfaceVoltageTooHigh(const IPG_USHORT& value) { interfaceVoltageTooHigh = value; }

    IPG_USHORT getBackReflectionAlarm() const { return backReflectionAlarm; }

    void setBackReflectionAlarm(const IPG_USHORT& value) { backReflectionAlarm = value; }

    IPG_USHORT getOpticalLinkAlarm() const { return opticalLinkAlarm; }

    void setOpticalLinkAlarm(const IPG_USHORT& value) { opticalLinkAlarm = value; }

    IPG_USHORT getProcessFiberAlarm() const { return processFiberAlarm; }

    void setProcessFiberAlarm(const IPG_USHORT& value) { processFiberAlarm = value; }

    IPG_USHORT getFeedingFiberAlarm() const { return FeedingFiberAlarm; }

    void setFeedingFiberAlarm(const IPG_USHORT& value) { FeedingFiberAlarm = value; }

    IPG_USHORT getHotStop() const { return hotStop; }

    void setHotStop(const IPG_USHORT& value) { hotStop = value; }

    IPG_USHORT getInterfacePSHotStop() const { return interfacePSHotStop; }

    void setInterfacePSHotStop(const IPG_USHORT& value) { interfacePSHotStop = value; }

    IPG_USHORT getMainPSHotStop() const { return mainPSHotStop; }

    void setMainPSHotStop(const IPG_USHORT& value) { mainPSHotStop = value; }

    IPG_USHORT getOpticalLinkHotStop() const { return opticalLinkHotStop; }

    void setOpticalLinkHotStop(const IPG_USHORT& value) { opticalLinkHotStop = value; }

    IPG_USHORT getSafetyOpenWhileEmission() const { return safetyOpenWhileEmission; }

    void setSafetyOpenWhileEmission(const IPG_USHORT& value) { safetyOpenWhileEmission = value; }

    IPG_USHORT getExternalStopWhileEmission() const { return externalStopWhileEmission; }

    void setExternalStopWhileEmission(const IPG_USHORT& value) { externalStopWhileEmission = value; }

    IPG_USHORT getGLWhileEmission() const { return GLWhileEmission; }

    void setGLWhileEmission(const IPG_USHORT& value) { GLWhileEmission = value; }

    IPG_USHORT getSystemAlarm() const { return systemAlarm; }

    void setSystemAlarm(const IPG_USHORT& value) { systemAlarm = value; }

    IPG_USHORT getLaserInterface() const { return laserInterface; }

    void setLaserInterface(const IPG_USHORT& value) { laserInterface = value; }

    IPG_USHORT getOpticalModule1() const { return opticalModule1; }

    void setOpticalModule1(const IPG_USHORT& value) { opticalModule1 = value; }

    IPG_USHORT getOpticalModule2() const { return opticalModule2; }

    void setOpticalModule2(const IPG_USHORT& value) { opticalModule2 = value; }

    IPG_USHORT getPowerSupply() const { return powerSupply; }

    void setPowerSupply(const IPG_USHORT& value) { powerSupply = value; }

    IPG_USHORT getSafety() const { return safety; }

    void setSafety(const IPG_USHORT& value) { safety = value; }

    IPG_USHORT getCommunicationOM1() const { return communicationOM1; }

    void setCommunicationOM1(const IPG_USHORT& value) { communicationOM1 = value; }

    IPG_USHORT getCommunicationOM2() const { return communicationOM2; }

    void setCommunicationOM2(const IPG_USHORT& value) { communicationOM2 = value; }

    IPG_USHORT getCommunicationPS() const { return communicationPS; }

    void setCommunicationPS(const IPG_USHORT& value) { communicationPS = value; }

    IPG_USHORT getCommunicationSafety() const { return communicationSafety; }

    void setCommunicationSafety(const IPG_USHORT& value) { communicationSafety = value; }

    IPG_USHORT getCriticalAlarm() const { return criticalAlarm; }

    void setCriticalAlarm(const IPG_USHORT& value) { criticalAlarm = value; }

    IPG_USHORT getTemperatureAlarm() const { return temperatureAlarm; }

    void setTemperatureAlarm(const IPG_USHORT& value) { temperatureAlarm = value; }

    IPG_USHORT getInterfaceTemperatureTooLow() const { return interfaceTemperatureTooLow; }

    void setInterfaceTemperatureTooLow(const IPG_USHORT& value) { interfaceTemperatureTooLow = value; }

    IPG_USHORT getInterfaceTemperatureTooHigh() const { return interfaceTemperatureTooHigh; }

    void setInterfaceTemperatureTooHigh(const IPG_USHORT& value) { interfaceTemperatureTooHigh = value; }

    IPG_USHORT getLaserTemperatureTooLow() const { return laserTemperatureTooLow; }

    void setLaserTemperatureTooLow(const IPG_USHORT& value) { laserTemperatureTooLow = value; }

    IPG_USHORT getLaserTemperatureTooHigh() const { return laserTemperatureTooHigh; }

    void setLaserTemperatureTooHigh(const IPG_USHORT& value) { laserTemperatureTooHigh = value; }

    IPG_USHORT getReserved2() const { return reserved2; }

    void setReserved2(const IPG_USHORT& value) { reserved2 = value; }

    IPG_USHORT getReserved3() const { return reserved3; }

    void setReserved3(const IPG_USHORT& value) { reserved3 = value; }

    IPG_USHORT getReserved4() const { return reserved4; }

    void setReserved4(const IPG_USHORT& value) { reserved4 = value; }

    IPG_USHORT getReserved5() const { return reserved5; }

    void setReserved5(const IPG_USHORT& value) { reserved5 = value; }

    IPG_USHORT getReserved1() const { return reserved1; }

    void setReserved1(const IPG_USHORT& value) { reserved1 = value; }

};


}

Q_DECLARE_METATYPE(ipg::GetAlarmCounterOutput)

#endif // GET_ALARM_COUNTER_HPP
