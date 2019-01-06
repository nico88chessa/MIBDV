#ifndef GET_ALARM_COUNTER_MARSHALLER_HPP
#define GET_ALARM_COUNTER_MARSHALLER_HPP

#include "../beans/GetAlarmCounter.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class GetAlarmCounterMarshaller : public IpgMarshaller<GetAlarmCounterInput, GetAlarmCounterOutput>{
public:
    typedef GetAlarmCounterMarshaller* Ptr;
    typedef const GetAlarmCounterMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetAlarmCounterOutput::Ptr output) {

//        const int offsets[38] = {
//            0,
//            2,
//            4,
//            6,
//            8,
//            10,
//            12,
//            14,
//            16,
//            18,
//            20,
//            22,
//            24,
//            26,
//            28,
//            30,
//            32,
//            34,
//            36,
//            38,
//            40,
//            42,
//            44,
//            46,
//            48,
//            50,
//            52,
//            54,
//            56,
//            58,
//            60,
//            62,
//            64,
//            66,
//            68,
//            70,
//            72,
//            74
//        };

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

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> version;
        stream >> systemAlarm;
        stream >> laserInterface;
        stream >> opticalModule1;
        stream >> opticalModule2;
        stream >> powerSupply;
        stream >> safety;
        stream >> communicationOM1;
        stream >> communicationOM2;
        stream >> communicationPS;
        stream >> communicationSafety;
        stream >> criticalAlarm;
        stream >> temperatureAlarm;
        stream >> interfaceTemperatureTooLow;
        stream >> interfaceTemperatureTooHigh;
        stream >> laserTemperatureTooLow;
        stream >> laserTemperatureTooHigh;
        stream >> reserved1;
        stream >> reserved2;
        stream >> psTemperatureTooHigh;
        stream >> reserved3;
        stream >> interfaceController24VAlarm;
        stream >> interfaceVoltageTooLow;
        stream >> interfaceVoltageTooHigh;
        stream >> backReflectionAlarm;
        stream >> opticalLinkAlarm;
        stream >> processFiberAlarm;
        stream >> FeedingFiberAlarm;
        stream >> hotStop;
        stream >> interfacePSHotStop;
        stream >> mainPSHotStop;
        stream >> opticalLinkHotStop;
        stream >> safetyOpenWhileEmission;
        stream >> externalStopWhileEmission;
        stream >> GLWhileEmission;
        stream >> reserved4;
        stream >> reserved5;

        output->setVersion(version);
        output->setSystemAlarm(systemAlarm);
        output->setLaserInterface(laserInterface);
        output->setOpticalModule1(opticalModule1);
        output->setOpticalModule2(opticalModule2);
        output->setPowerSupply(powerSupply);
        output->setSafety(safety);
        output->setCommunicationOM1(communicationOM1);
        output->setCommunicationOM2(communicationOM2);
        output->setCommunicationPS(communicationPS);
        output->setCommunicationSafety(communicationSafety);
        output->setCriticalAlarm(criticalAlarm);
        output->setTemperatureAlarm(temperatureAlarm);
        output->setInterfaceTemperatureTooLow(interfaceTemperatureTooLow);
        output->setInterfaceTemperatureTooHigh(interfaceTemperatureTooHigh);
        output->setLaserTemperatureTooLow(laserTemperatureTooLow);
        output->setLaserTemperatureTooHigh(laserTemperatureTooHigh);
        output->setReserved1(reserved1);
        output->setReserved2(reserved2);
        output->setPsTemperatureTooHigh(psTemperatureTooHigh);
        output->setReserved3(reserved3);
        output->setInterfaceController24VAlarm(interfaceController24VAlarm);
        output->setInterfaceVoltageTooLow(interfaceVoltageTooLow);
        output->setInterfaceVoltageTooHigh(interfaceVoltageTooHigh);
        output->setBackReflectionAlarm(backReflectionAlarm);
        output->setOpticalLinkAlarm(opticalLinkAlarm);
        output->setProcessFiberAlarm(processFiberAlarm);
        output->setFeedingFiberAlarm(FeedingFiberAlarm);
        output->setHotStop(hotStop);
        output->setInterfacePSHotStop(interfacePSHotStop);
        output->setMainPSHotStop(mainPSHotStop);
        output->setOpticalLinkHotStop(opticalLinkHotStop);
        output->setSafetyOpenWhileEmission(safetyOpenWhileEmission);
        output->setExternalStopWhileEmission(externalStopWhileEmission);
        output->setGLWhileEmission(GLWhileEmission);
        output->setReserved4(reserved4);
        output->setReserved5(reserved5);


        /*bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        systemAlarm = bytes.mid(offsets[1], offsets[2] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        laserInterface = bytes.mid(offsets[2], offsets[3] - offsets[2]).toUShort(&ok);
        if (!ok) return false;

        opticalModule1 = bytes.mid(offsets[3], offsets[4] - offsets[3]).toUShort(&ok);
        if (!ok) return false;

        opticalModule2 = bytes.mid(offsets[4], offsets[5] - offsets[4]).toUShort(&ok);
        if (!ok) return false;

        powerSupply = bytes.mid(offsets[5], offsets[6] - offsets[5]).toUShort(&ok);
        if (!ok) return false;

        safety = bytes.mid(offsets[6], offsets[7] - offsets[6]).toUShort(&ok);
        if (!ok) return false;

        communicationOM1 = bytes.mid(offsets[7], offsets[8] - offsets[7]).toUShort(&ok);
        if (!ok) return false;

        communicationOM2 = bytes.mid(offsets[8], offsets[9] - offsets[8]).toUShort(&ok);
        if (!ok) return false;

        communicationPS = bytes.mid(offsets[9], offsets[10] - offsets[9]).toUShort(&ok);
        if (!ok) return false;

        communicationSafety = bytes.mid(offsets[10], offsets[11] - offsets[10]).toUShort(&ok);
        if (!ok) return false;

        criticalAlarm = bytes.mid(offsets[11], offsets[12] - offsets[11]).toUShort(&ok);
        if (!ok) return false;

        temperatureAlarm = bytes.mid(offsets[12], offsets[13] - offsets[12]).toUShort(&ok);
        if (!ok) return false;

        interfaceTemperatureTooLow = bytes.mid(offsets[13], offsets[14] - offsets[13]).toUShort(&ok);
        if (!ok) return false;

        interfaceTemperatureTooHigh = bytes.mid(offsets[14], offsets[15] - offsets[14]).toUShort(&ok);
        if (!ok) return false;

        laserTemperatureTooLow = bytes.mid(offsets[15], offsets[16] - offsets[15]).toUShort(&ok);
        if (!ok) return false;

        laserTemperatureTooHigh = bytes.mid(offsets[16], offsets[17] - offsets[16]).toUShort(&ok);
        if (!ok) return false;

        psTemperatureTooHigh = bytes.mid(offsets[19], offsets[20] - offsets[19]).toUShort(&ok);
        if (!ok) return false;

        interfaceController24VAlarm = bytes.mid(offsets[21], offsets[22] - offsets[21]).toUShort(&ok);
        if (!ok) return false;

        interfaceVoltageTooLow = bytes.mid(offsets[22], offsets[23] - offsets[22]).toUShort(&ok);
        if (!ok) return false;

        interfaceVoltageTooHigh = bytes.mid(offsets[23], offsets[24] - offsets[23]).toUShort(&ok);
        if (!ok) return false;

        backReflectionAlarm = bytes.mid(offsets[24], offsets[25] - offsets[24]).toUShort(&ok);
        if (!ok) return false;

        opticalLinkAlarm = bytes.mid(offsets[25], offsets[26] - offsets[25]).toUShort(&ok);
        if (!ok) return false;

        processFiberAlarm = bytes.mid(offsets[26], offsets[27] - offsets[26]).toUShort(&ok);
        if (!ok) return false;

        FeedingFiberAlarm = bytes.mid(offsets[27], offsets[28] - offsets[27]).toUShort(&ok);
        if (!ok) return false;

        hotStop = bytes.mid(offsets[28], offsets[29] - offsets[28]).toUShort(&ok);
        if (!ok) return false;

        interfacePSHotStop = bytes.mid(offsets[29], offsets[30] - offsets[29]).toUShort(&ok);
        if (!ok) return false;

        mainPSHotStop = bytes.mid(offsets[30], offsets[31] - offsets[30]).toUShort(&ok);
        if (!ok) return false;

        opticalLinkHotStop = bytes.mid(offsets[31], offsets[32] - offsets[31]).toUShort(&ok);
        if (!ok) return false;

        safetyOpenWhileEmission = bytes.mid(offsets[32], offsets[33] - offsets[32]).toUShort(&ok);
        if (!ok) return false;

        externalStopWhileEmission = bytes.mid(offsets[33], offsets[34] - offsets[33]).toUShort(&ok);
        if (!ok) return false;

        GLWhileEmission = bytes.mid(offsets[34], offsets[35] - offsets[34]).toUShort(&ok);
        if (!ok) return false;

        output->setVersion(version);
        output->setSystemAlarm(systemAlarm);
        output->setLaserInterface(laserInterface);
        output->setOpticalModule1(opticalModule1);
        output->setOpticalModule2(opticalModule2);
        output->setPowerSupply(powerSupply);
        output->setSafety(safety);
        output->setCommunicationOM1(communicationOM1);
        output->setCommunicationOM2(communicationOM2);
        output->setCommunicationPS(communicationPS);
        output->setCommunicationSafety(communicationSafety);
        output->setCriticalAlarm(criticalAlarm);
        output->setTemperatureAlarm(temperatureAlarm);
        output->setInterfaceTemperatureTooLow(interfaceTemperatureTooLow);
        output->setInterfaceTemperatureTooHigh(interfaceTemperatureTooHigh);
        output->setLaserTemperatureTooLow(laserTemperatureTooLow);
        output->setLaserTemperatureTooHigh(laserTemperatureTooHigh);
        output->setPsTemperatureTooHigh(psTemperatureTooHigh);
        output->setInterfaceController24VAlarm(interfaceController24VAlarm);
        output->setInterfaceVoltageTooLow(interfaceVoltageTooLow);
        output->setInterfaceVoltageTooHigh(interfaceVoltageTooHigh);
        output->setBackReflectionAlarm(backReflectionAlarm);
        output->setOpticalLinkAlarm(opticalLinkAlarm);
        output->setProcessFiberAlarm(processFiberAlarm);
        output->setFeedingFiberAlarm(FeedingFiberAlarm);
        output->setHotStop(hotStop);
        output->setInterfacePSHotStop(interfacePSHotStop);
        output->setMainPSHotStop(mainPSHotStop);
        output->setOpticalLinkHotStop(opticalLinkHotStop);
        output->setSafetyOpenWhileEmission(safetyOpenWhileEmission);
        output->setExternalStopWhileEmission(externalStopWhileEmission);
        output->setGLWhileEmission(GLWhileEmission);
        */

        return true;

    }

};

}

#endif // GET_ALARM_COUNTER_MARSHALLER_HPP
