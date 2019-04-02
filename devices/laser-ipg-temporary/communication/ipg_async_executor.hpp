#ifndef IPG_LASER_TEMP_IPG_ASYNC_EXECUTOR_HPP
#define IPG_LASER_TEMP_IPG_ASYNC_EXECUTOR_HPP

#include <Logger.hpp>

#include <laser-ipg-temporary/communication/abstract_async_executor.hpp>
#include <laser-ipg-temporary/marshalling/marshallers.hpp>

namespace ipg {

static const int IPG_ASYNC_EXECUTOR_HEADER_RECEIVE_ERROR = 1;
static const int IPG_ASYNC_EXECUTOR_DATA_RECEIVE_ERROR = 2;
static const int IPG_ASYNC_EXECUTOR_CRC16_ERROR = 3;
static const int IPG_ASYNC_EXECUTOR_MARSHALLER_FACTORY_ERROR = 4;
static const int IPG_ASYNC_EXECUTOR_BEAN_FACTORY_ERROR = 5;
static const int IPG_ASYNC_EXECUTOR_UNMARSHALL_ERROR = 6;

class IpgAsyncExecutor : public AbstractAsyncExecutor {
    Q_OBJECT

public:
    typedef IpgAsyncExecutor* Ptr;
    typedef const IpgAsyncExecutor* ConstPtr;

protected:

    MarshallerInterface::Ptr marshallerFactory(int i) {

        MarshallerInterface::Ptr marshaller = NULL;

        switch (i) {
        case COMMAND_GET_NETWORK_SETTINGS: marshaller = new GetNetworkSettingsMarshaller(); break;
        case COMMAND_SET_IPSETTINGS: marshaller = new SetIpSettingsMarshaller(); break;
        case COMMAND_SET_COMMAND_TIMEOUT: marshaller = new SetCommandTimeoutMarshaller(); break;
        case COMMAND_GET_CRITICAL_ERROR_COUNTER: marshaller = new GetCriticalErrorCounterMarshaller(); break;
        case COMMAND_GET_LASER_MODEL_INFO: marshaller = new GetLaserModelInfoMarshaller(); break;
        case COMMAND_GET_LASER_SOFTWARE_INFO: marshaller = new GetLaserSoftwareInfoMarshaller(); break;
        case COMMAND_GET_MODE_PARAMETER: marshaller = new GetModeParameterMarshaller(); break;
        case COMMAND_GET_ALARM_COUNTER: marshaller = new GetAlarmCounterMarshaller(); break;
        case COMMAND_SET_LASER_INTERFACE: marshaller = new SetLaserInterfaceMarshaller(); break;
        case COMMAND_GET_LASER_INTERFACE: marshaller = new GetLaserInterfaceMarshaller(); break;
        case COMMAND_RESET: marshaller = new ResetMarshaller(); break;
        case COMMAND_RESET_CRITICAL_ERROR: marshaller = new ResetCriticalErrorMarshaller(); break;
        case COMMAND_SET_MODE_INDEX: marshaller = new SetModeIndexMarshaller(); break;
        case COMMAND_SET_PS_MODE: marshaller = new SetPSModeMarshaller(); break;
        case COMMAND_SET_GL: marshaller = new SetGLModeMarshaller(); break;
        case COMMAND_SET_FREQUENCY: marshaller = new SetFrequencyMarshaller(); break;
        case COMMAND_SET_POWER: marshaller = new SetPowerMarshaller(); break;
        case COMMAND_SET_EE: marshaller = new SetEEMarshaller(); break;
        case COMMAND_SET_EM: marshaller = new SetEMMarshaller(); break;
        case COMMAND_GET_LASER_STATUS: marshaller = new GetLaserStatusMarshaller(); break;
        default: break;
        }

        return marshaller;

    }

    OutputBean::Ptr outputBeanFactory(int i) {

        OutputBean::Ptr bean = NULL;

        switch (i) {
        case COMMAND_GET_NETWORK_SETTINGS: bean = new GetNetworkSettingsOutput(); break;
        case COMMAND_SET_IPSETTINGS: bean = new SetIpSettingsOutput(); break;
        case COMMAND_SET_COMMAND_TIMEOUT: bean = new SetCommandTimeoutOutput(); break;
        case COMMAND_GET_CRITICAL_ERROR_COUNTER: bean = new GetCriticalErrorCounterOutput(); break;
        case COMMAND_GET_LASER_MODEL_INFO: bean = new GetLaserModelInfoOutput(); break;
        case COMMAND_GET_LASER_SOFTWARE_INFO: bean = new GetLaserSoftwareInfoOutput(); break;
        case COMMAND_GET_MODE_PARAMETER: bean = new GetModeParameterOutput(); break;
        case COMMAND_GET_ALARM_COUNTER: bean = new GetAlarmCounterOutput(); break;
        case COMMAND_SET_LASER_INTERFACE: bean = new SetLaserInterfaceOutput(); break;
        case COMMAND_GET_LASER_INTERFACE: bean = new GetLaserInterfaceOutput(); break;
        case COMMAND_RESET: bean = new ResetOutput(); break;
        case COMMAND_RESET_CRITICAL_ERROR: bean = new ResetCriticalErrorOutput(); break;
        case COMMAND_SET_MODE_INDEX: bean = new SetModeIndexOutput(); break;
        case COMMAND_SET_PS_MODE: bean = new SetPSModeOutput(); break;
        case COMMAND_SET_GL: bean = new SetGLModeOutput(); break;
        case COMMAND_SET_FREQUENCY: bean = new SetFrequencyOutput(); break;
        case COMMAND_SET_POWER: bean = new SetPowerOutput(); break;
        case COMMAND_SET_EE: bean = new SetEEOutput(); break;
        case COMMAND_SET_EM: bean = new SetEMOutput(); break;
        case COMMAND_GET_LASER_STATUS: bean = new GetLaserStatusOutput(); break;
        default: break;
        }

        return bean;

    }

public:

    IpgAsyncExecutor(AbstractSender::Ptr _s, AbstractReceiver::Ptr _r, QObject* parent=0) :
        AbstractAsyncExecutor(_s, _r, parent) { }

public slots:

    virtual int receive() {

        using namespace PROGRAM_NAMESPACE;

        const int offsets[4] = {0, 2, 4, 8};

        while (receiver->hasData()) {

            QByteArray stream1; // stream1 riguarda l'header

            // qui leggo l'header
            int errorCode = receiver->readBytes(stream1, offsets[3]);
            if ( errorCode > 0 ) {
                traceWarn() << "Ipg async executor: errore nella lettura dell'header con codice errore:" << errorCode;
                return IPG_ASYNC_EXECUTOR_HEADER_RECEIVE_ERROR;
            }

            QDataStream streamHeader(stream1);
            streamHeader.setByteOrder(QDataStream::LittleEndian);

            IPG_USHORT answer;
            IPG_USHORT executionCode;
            IPG_ULONG dataSize;

            streamHeader >> answer;
            streamHeader >> executionCode;
            streamHeader >> dataSize;

            // qui leggo i dati e il CRC 16
            QByteArray stream2; // stream2 riguarda la parte dati e il codice CRC16
            errorCode = receiver->readBytes(stream2, dataSize+2);
            if (errorCode > 0) {
                traceWarn() << "Ipg async executor: errore nella lettura dei dati con codice errore:" << errorCode;
                return IPG_ASYNC_EXECUTOR_DATA_RECEIVE_ERROR;
            }

            // qui controllo il codice CRC 16
            QDataStream streamCrc16(stream2.mid(dataSize, 2));
            streamCrc16.setByteOrder(QDataStream::LittleEndian);

            IPG_USHORT crc16Bytes;
            streamCrc16 >> crc16Bytes;

            QByteArray fullData;
            fullData.append(stream1);
            fullData.append(stream2);

            // controllo che il crc16 sia lo stesso fra inviato e calcolato
//            const QByteArray& data = fullData.mid(0, dataSize);
            if (crc16Bytes != getCrc16(reinterpret_cast<const unsigned char*>(fullData.constData()), fullData.size()-2)) {
                traceWarn() << "Ipg async executor: errore nel controllo codice CRC16.";
                return IPG_ASYNC_EXECUTOR_CRC16_ERROR;
            }

            MarshallerInterface::Ptr marshaller = this->marshallerFactory(answer);
            if (marshaller == NULL) {
                traceWarn() << "Ipg async executor: errore nel factoring marshaller.";
                return IPG_ASYNC_EXECUTOR_MARSHALLER_FACTORY_ERROR;
            }

            OutputBean::Ptr outputBean = this->outputBeanFactory(answer);
            if (outputBean == NULL) {
                traceWarn() << "Ipg async executor: errore nel factoring bean.";
                return IPG_ASYNC_EXECUTOR_BEAN_FACTORY_ERROR;
            }

            if (dataSize > 0)
                if (!marshaller->unmarshall(fullData, outputBean)) {
                    traceWarn() << "Ipg async executor: errore nella fase di unmarshaller.";
                    return IPG_ASYNC_EXECUTOR_UNMARSHALL_ERROR;
                }

            switch (answer) {
            case COMMAND_GET_NETWORK_SETTINGS: {
                GetNetworkSettingsOutput::Ptr gns = dynamic_cast<GetNetworkSettingsOutput::Ptr>(outputBean);
                if (gns != NULL)
                    emit getNetworkSettingsSignal(*gns);
                break;
            }
            case COMMAND_SET_IPSETTINGS: {
                SetIpSettingsOutput::Ptr si = dynamic_cast<SetIpSettingsOutput::Ptr>(outputBean);
                if (si != NULL)
                    emit setIpSettingsSignal(*si);
                break;
            }
            case COMMAND_SET_COMMAND_TIMEOUT: {
                SetCommandTimeoutOutput::Ptr sct = dynamic_cast<SetCommandTimeoutOutput::Ptr>(outputBean);
                if (sct != NULL)
                    emit setCommandTimeoutSignal(*sct);
                break;
            }
            case COMMAND_GET_CRITICAL_ERROR_COUNTER: {
                GetCriticalErrorCounterOutput::Ptr gcec = dynamic_cast<GetCriticalErrorCounterOutput::Ptr>(outputBean);
                if (gcec != NULL)
                    emit getCriticalErrorCounterSignal(*gcec);
                break;
            }
            case COMMAND_GET_LASER_MODEL_INFO: {
                GetLaserModelInfoOutput::Ptr glmi = dynamic_cast<GetLaserModelInfoOutput::Ptr>(outputBean);
                if (glmi != NULL)
                    emit getLaserModelInfoSignal(*glmi);
                break;
            }
            case COMMAND_GET_LASER_SOFTWARE_INFO: {
                GetLaserSoftwareInfoOutput::Ptr glsi = dynamic_cast<GetLaserSoftwareInfoOutput::Ptr>(outputBean);
                if (glsi != NULL)
                    emit getLaserSoftwareInfoSignal(*glsi);
                break;
            }
            case COMMAND_GET_MODE_PARAMETER: {
                GetModeParameterOutput::Ptr gmp = dynamic_cast<GetModeParameterOutput::Ptr>(outputBean);
                if (gmp != NULL)
                    emit getModeParameterSignal(*gmp);
                break;
            }
            case COMMAND_GET_ALARM_COUNTER: {
                GetAlarmCounterOutput::Ptr gac = dynamic_cast<GetAlarmCounterOutput::Ptr>(outputBean);
                if (gac != NULL)
                    emit getAlarmCounterSignal(*gac);
                break;
            }
            case COMMAND_SET_LASER_INTERFACE: {
                SetLaserInterfaceOutput::Ptr sli = dynamic_cast<SetLaserInterfaceOutput::Ptr>(outputBean);
                if (sli != NULL)
                    emit setLaserInterfaceSignal(*sli);
                break;
            }
            case COMMAND_GET_LASER_INTERFACE: {
                GetLaserInterfaceOutput::Ptr gli = dynamic_cast<GetLaserInterfaceOutput::Ptr>(outputBean);
                if (gli != NULL)
                    emit getLaserInterfaceSignal(*gli);
                break;
            }
            case COMMAND_RESET: {
                ResetOutput::Ptr r = dynamic_cast<ResetOutput::Ptr>(outputBean);
                if (r != NULL)
                    emit resetSignal(*r);
                break;
            }
            case COMMAND_RESET_CRITICAL_ERROR: {
                ResetCriticalErrorOutput::Ptr rce = dynamic_cast<ResetCriticalErrorOutput::Ptr>(outputBean);
                if (rce != NULL)
                    emit resetCriticalErrorSignal(*rce);
                break;
            }
            case COMMAND_SET_MODE_INDEX: {
                SetModeIndexOutput::Ptr smi = dynamic_cast<SetModeIndexOutput::Ptr>(outputBean);
                if (smi != NULL)
                    emit setModeIndexSignal(*smi);
                break;
            }
            case COMMAND_SET_PS_MODE: {
                SetPSModeOutput::Ptr spm = dynamic_cast<SetPSModeOutput::Ptr>(outputBean);
                if (spm != NULL)
                    emit setPSModeSignal(*spm);
                break;
            }
            case COMMAND_SET_GL: {
                SetGLModeOutput::Ptr sg = dynamic_cast<SetGLModeOutput::Ptr>(outputBean);
                if (sg != NULL)
                    emit setGLModeSignal(*sg);
                break;
            }
            case COMMAND_SET_FREQUENCY: {
                SetFrequencyOutput::Ptr sf = dynamic_cast<SetFrequencyOutput::Ptr>(outputBean);
                if (sf != NULL)
                    emit setFrequencySignal(*sf);
                break;
            }
            case COMMAND_SET_POWER: {
                SetPowerOutput::Ptr sp = dynamic_cast<SetPowerOutput::Ptr>(outputBean);
                if (sp != NULL)
                    emit setPowerSignal(*sp);
                break;
            }
            case COMMAND_SET_EE: {
                SetEEOutput::Ptr see = dynamic_cast<SetEEOutput::Ptr>(outputBean);
                if (see != NULL)
                    emit setEESignal(*see);
                break;
            }
            case COMMAND_SET_EM: {
                SetEMOutput::Ptr sem = dynamic_cast<SetEMOutput::Ptr>(outputBean);
                if (sem != NULL)
                    emit setEMSignal(*sem);
                break;
            }
            case COMMAND_GET_LASER_STATUS: {
                GetLaserStatusOutput::Ptr g = dynamic_cast<GetLaserStatusOutput::Ptr>(outputBean);
                if (g != NULL)
                    emit getLaserStatusSignal(*g);
                break;
            }
            default: break;
            }

            delete marshaller;
            delete outputBean;

        }

        return true;

    }

signals:
    void getNetworkSettingsSignal(ipg::GetNetworkSettingsOutput);
    void setIpSettingsSignal(ipg::SetIpSettingsOutput);
    void setCommandTimeoutSignal(ipg::SetCommandTimeoutOutput);
    void getCriticalErrorCounterSignal(ipg::GetCriticalErrorCounterOutput);
    void getLaserModelInfoSignal(ipg::GetLaserModelInfoOutput);
    void getLaserSoftwareInfoSignal(ipg::GetLaserSoftwareInfoOutput);
    void getModeParameterSignal(ipg::GetModeParameterOutput);
    void getAlarmCounterSignal(ipg::GetAlarmCounterOutput);
    void setLaserInterfaceSignal(ipg::SetLaserInterfaceOutput);
    void getLaserInterfaceSignal(ipg::GetLaserInterfaceOutput);
    void resetSignal(ipg::ResetOutput);
    void resetCriticalErrorSignal(ipg::ResetCriticalErrorOutput);
    void setModeIndexSignal(ipg::SetModeIndexOutput);
    void setPSModeSignal(ipg::SetPSModeOutput);
    void setGLModeSignal(ipg::SetGLModeOutput);
    void setFrequencySignal(ipg::SetFrequencyOutput);
    void setPowerSignal(ipg::SetPowerOutput);
    void setEESignal(ipg::SetEEOutput);
    void setEMSignal(ipg::SetEMOutput);
    void getLaserStatusSignal(ipg::GetLaserStatusOutput);

};

}

#endif // IPG_ASYNC_EXECUTOR_HPP
