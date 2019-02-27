#ifndef IPG_SYNC_INTERFACE_HPP
#define IPG_SYNC_INTERFACE_HPP

#include <QHostAddress>
#include <QPointer>

#include <communication/ethernet/SocketSyncExecutor.hpp>
#include "../marshalling/Marshallers.hpp"

namespace ipg {

namespace ce = communication::ethernet;

class IpgSyncInterface : public QObject {
    Q_OBJECT

public:
    typedef IpgSyncInterface* Ptr;
    typedef const IpgSyncInterface* ConstPtr;

private:
    QPointer<ce::AbstractSyncExecutor> syncExecutor;
    bool isConnected;

private slots:

    void socketErrorTrace(QAbstractSocket::SocketError er) {
        qDebug() << "  - Ipg sync interface: errore socket; codice errore: " << er;
    }

    void socketConnected() {
        this->isConnected = true;
        qDebug() << "  - Ipg sync interface: connessione avvenuto con successo. ";
    }

    void socketDisconnected() {
        this->isConnected = false;
        qDebug() << "  - Ipg sync interface: disconnessione comunicazione.";
    }

public:

    IpgSyncInterface(QObject* parent = nullptr) : QObject(parent), isConnected(false) {

        this->syncExecutor = new ce::SocketSyncExecutor();

        connect(syncExecutor, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketErrorTrace(QAbstractSocket::SocketError)));
        connect(syncExecutor, SIGNAL(connected()), this, SLOT(socketConnected()));
        connect(syncExecutor, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    }

    ~IpgSyncInterface() {

        if (!syncExecutor.isNull()) {
            static_cast<ce::SocketSyncExecutor::Ptr>(syncExecutor.data())->closeConnection();
            delete syncExecutor;
        }

    }

    bool connectToLaser(const QString& ip, quint16 port) {

        static_cast<ce::SocketSyncExecutor::Ptr>(this->syncExecutor.data())->setupConnection(ip, port);

        if (!static_cast<ce::SocketSyncExecutor::Ptr>(syncExecutor.data())->createConnection())
            return false;

        return true;

    }

    bool disconnectLaser() {

        if (!syncExecutor.isNull())
            static_cast<ce::SocketSyncExecutor::Ptr>(this->syncExecutor.data())->closeConnection();

        return true;

    }

    bool getLaserId(GetLaserIdOutput& laserId, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetLaserIdInput input;
        input.setCommand(COMMAND_GET_LASERID);
        if (syncExecutor->execute<GetLaserIdInput, GetLaserIdOutput, GetLaserIdMarshaller>(input, laserId) > 0)
            return false;

        executionCode = laserId.getExecutionCode();
        return true;

    }

    bool getNetworkSettings(GetNetworkSettingsOutput& networkSettings, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetNetworkSettingsInput input;
        input.setCommand(COMMAND_GET_NETWORK_SETTINGS);
        if (syncExecutor->execute<GetNetworkSettingsInput, GetNetworkSettingsOutput, GetNetworkSettingsMarshaller>(input, networkSettings) > 0)
            return false;

        executionCode = networkSettings.getExecutionCode();
        return true;

    }

    bool setIpSettings(const QString& ip, quint16 port, const QString& mask, const QString& gateway, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        QHostAddress ipAddress(ip);
        IPG_ULONG ipAddressNumber = qToBigEndian(ipAddress.toIPv4Address());

        QHostAddress gatewayAddress(gateway);
        IPG_ULONG gatewayNumber = qToBigEndian(gatewayAddress.toIPv4Address());

        QHostAddress maskAddress(mask);
        IPG_ULONG maskNumber = qToBigEndian(maskAddress.toIPv4Address());

        // TODO IPG: verificare maschera configurazione;
        IPG_USHORT config = 0x03;
        config &= ~NETWORK_CONFIGURATION_MASK_DHCP_ENABLED;
        config &= ~NETWORK_CONFIGURATION_MASK_FTP_ENABLED;

        SetIpSettingsInput input;
        SetIpSettingsOutput output;

        input.setCommand(COMMAND_SET_IPSETTINGS);
        input.setVersion(1);
        input.setIpAddress(ipAddressNumber);
        input.setDefaultGateway(gatewayNumber);
        input.setIpMask(maskNumber);
        input.setPort(port);
        input.setConfig(config);

        if (syncExecutor->execute<SetIpSettingsInput, SetIpSettingsOutput, SetIpSettingsMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool setCommandTimeout(quint32 timeoutMs, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetCommandTimeoutInput input;
        SetCommandTimeoutOutput output;

        input.setCommand(COMMAND_SET_COMMAND_TIMEOUT);
        input.setTimeout(timeoutMs);

        if (syncExecutor->execute<SetCommandTimeoutInput, SetCommandTimeoutOutput, SetCommandTimeoutMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool getCriticalErrorCounter(GetCriticalErrorCounterOutput& criticalErrorCounter, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetCriticalErrorCounterInput input;
        input.setCommand(COMMAND_GET_CRITICAL_ERROR_COUNTER);
        if (syncExecutor->execute<GetCriticalErrorCounterInput, GetCriticalErrorCounterOutput, GetCriticalErrorCounterMarshaller>(input, criticalErrorCounter) > 0)
            return false;

        executionCode = criticalErrorCounter.getExecutionCode();
        return true;

    }

    bool getLaserModelInfo(GetLaserModelInfoOutput& laserModelInfo, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetLaserModelInfoInput input;
        input.setCommand(COMMAND_GET_LASER_MODEL_INFO);
        if (syncExecutor->execute<GetLaserModelInfoInput, GetLaserModelInfoOutput, GetLaserModelInfoMarshaller>(input, laserModelInfo) > 0)
            return false;

        executionCode = laserModelInfo.getExecutionCode();
        return true;

    }

    bool getLaserSoftwareInfo(GetLaserSoftwareInfoOutput& laserSoftwareInfo, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetLaserSoftwareInfoInput input;
        input.setCommand(COMMAND_GET_LASER_SOFTWARE_INFO);
        if (syncExecutor->execute<GetLaserSoftwareInfoInput, GetLaserSoftwareInfoOutput, GetLaserSoftwareInfoMarshaller>(input, laserSoftwareInfo) > 0)
            return false;

        executionCode = laserSoftwareInfo.getExecutionCode();
        return true;

    }

    bool getModeParameter(quint16 indexMode, GetModeParameterOutput& getModeParameter, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetModeParameterInput input;

        input.setCommand(COMMAND_GET_MODE_PARAMETER);
        input.setModeIndex(indexMode);

        if (syncExecutor->execute<GetModeParameterInput, GetModeParameterOutput, GetModeParameterMarshaller>(input, getModeParameter) > 0)
            return false;

        executionCode = getModeParameter.getExecutionCode();
        return true;

    }

    bool getAlarmCounter(GetAlarmCounterOutput& alarmCounters, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetAlarmCounterInput input;
        input.setCommand(COMMAND_GET_ALARM_COUNTER);
        if (syncExecutor->execute<GetAlarmCounterInput, GetAlarmCounterOutput, GetAlarmCounterMarshaller>(input, alarmCounters) > 0)
            return false;

        executionCode = alarmCounters.getExecutionCode();
        return true;

    }

    bool setLaserInterface(
            quint16 mainInterface,
            quint16 powerConfig,
            quint16 eeConfig,
            quint16 emConfig,
            quint16 glConfig,
            quint16 prrConfig,
            quint16 psConfig,
            quint16 resetConfig,
            quint16 miConfig,
            SetLaserInterfaceOutput& result,
            IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetLaserInterfaceInput input;

        input.setVersion(1);
        input.setMode(0x0);
        input.setMainInterface(mainInterface);
        input.setPowerConfig(powerConfig);
        input.setEeConfig(eeConfig);
        input.setEmConfig(emConfig);
        input.setGlConfig(glConfig);
        input.setPrrConfig(prrConfig);
        input.setPsConfig(psConfig);
        input.setResetConfig(resetConfig);
        input.setMiConfig(miConfig);
        input.setCommand(COMMAND_SET_LASER_INTERFACE);

        if (syncExecutor->execute<SetLaserInterfaceInput, SetLaserInterfaceOutput, SetLaserInterfaceMarshaller>(input, result) > 0)
            return false;

        executionCode = result.getExecutionCode();
        return true;

    }

    bool saveLaserInterface(
            quint16 mainInterface,
            quint16 powerConfig,
            quint16 eeConfig,
            quint16 emConfig,
            quint16 glConfig,
            quint16 prrConfig,
            quint16 psConfig,
            quint16 resetConfig,
            quint16 miConfig,
            SetLaserInterfaceOutput& result,
            IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetLaserInterfaceInput input;

        input.setVersion(1);
        input.setMode(0x01);
        input.setMainInterface(mainInterface);
        input.setPowerConfig(powerConfig);
        input.setEeConfig(eeConfig);
        input.setEmConfig(emConfig);
        input.setGlConfig(glConfig);
        input.setPrrConfig(prrConfig);
        input.setPsConfig(psConfig);
        input.setResetConfig(resetConfig);
        input.setMiConfig(miConfig);
        input.setCommand(COMMAND_SET_LASER_INTERFACE);

        if (syncExecutor->execute<SetLaserInterfaceInput, SetLaserInterfaceOutput, SetLaserInterfaceMarshaller>(input, result) > 0)
            return false;

        executionCode = result.getExecutionCode();
        return true;

    }

    bool getLaserInterface(
            quint16& mainInterface,
            quint16& powerConfig,
            quint16& eeConfig,
            quint16& emConfig,
            quint16& glConfig,
            quint16& prrConfig,
            quint16& psConfig,
            quint16& resetConfig,
            quint16& miConfig,
            IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetLaserInterfaceInput input;
        GetLaserInterfaceOutput output;

        input.setCommand(COMMAND_GET_LASER_INTERFACE);
        if (syncExecutor->execute<GetLaserInterfaceInput, GetLaserInterfaceOutput, GetLaserInterfaceMarshaller>(input, output) > 0)
            return false;

        mainInterface = output.getMainInterface();
        powerConfig = output.getPowerConfig();
        eeConfig = output.getEeConfig();
        emConfig = output.getEmConfig();
        glConfig = output.getGlConfig();
        prrConfig = output.getPrrConfig();
        psConfig = output.getPsConfig();
        resetConfig = output.getResetConfig();
        miConfig = output.getMiConfig();
        executionCode = output.getExecutionCode();

        return true;

    }

    bool reset(IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        ResetInput input;
        ResetOutput output;
        input.setCommand(COMMAND_RESET);
        if (syncExecutor->execute<ResetInput, ResetOutput, ResetMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool resetCriticalError(const QString& ipgServiceString, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        ResetCriticalErrorInput input;
        ResetCriticalErrorOutput output;

        input.setCommand(COMMAND_RESET_CRITICAL_ERROR);
        input.setData(ipgServiceString);

        if (syncExecutor->execute<ResetCriticalErrorInput, ResetCriticalErrorOutput, ResetCriticalErrorMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool setModeIndex(quint16 mode, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetModeIndexInput input;
        SetModeIndexOutput output;

        input.setCommand(COMMAND_SET_MODE_INDEX);
        input.setModeIndex(mode);

        if (syncExecutor->execute<SetModeIndexInput, SetModeIndexOutput, SetModeIndexMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool setPsMode(bool powerSupply, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetPSModeInput input;
        SetPSModeOutput output;

        input.setCommand(COMMAND_SET_PS_MODE);
        input.setPowerSupply(powerSupply ? 1 : 0);

        if (syncExecutor->execute<SetPSModeInput, SetPSModeOutput, SetPSModeMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool setGlMode(bool guideLaser, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetGLModeInput input;
        SetGLModeOutput output;

        input.setCommand(COMMAND_SET_GL);
        input.setGlMode(guideLaser ? 1 : 0);

        if (syncExecutor->execute<SetGLModeInput, SetGLModeOutput, SetGLModeMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool setFrequency(float frequency, bool& result, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetFrequencyInput input;
        SetFrequencyOutput output;

        input.setCommand(COMMAND_SET_FREQUENCY);
        input.setFrequency(frequency);

        if (syncExecutor->execute<SetFrequencyInput, SetFrequencyOutput, SetFrequencyMarshaller>(input, output) > 0)
            return false;

        // TODO NIC: sentire longoni
//        result = output.getAnswer() == 0;
        result = true;
        executionCode = output.getExecutionCode();

        return true;

    }

    bool setPower(float power, bool& result, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetPowerInput input;
        SetPowerOutput output;

        input.setCommand(COMMAND_SET_POWER);
        input.setPower(power);

        if (syncExecutor->execute<SetPowerInput, SetPowerOutput, SetPowerMarshaller>(input, output) > 0)
            return false;

        // TODO NIC: sentire longoni
//        result = output.getAnswer() == 0;
        result = true;
        executionCode = output.getExecutionCode();

        return true;

    }

    bool setEE(bool emission, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetEEInput input;
        SetEEOutput output;

        input.setCommand(COMMAND_SET_EE);
        input.setEmissionOn(emission ? 1 : 0);

        if (syncExecutor->execute<SetEEInput, SetEEOutput, SetEEMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool setEM(bool modulation, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        SetEMInput input;
        SetEMOutput output;

        input.setCommand(COMMAND_SET_EM);
        input.setModulationOn(modulation ? 1 : 0);

        if (syncExecutor->execute<SetEMInput, SetEMOutput, SetEMMarshaller>(input, output) > 0)
            return false;

        executionCode = output.getExecutionCode();
        return true;

    }

    bool getLaserStatus(GetLaserStatusOutput& laserStatus, IPG_USHORT& executionCode) {

        executionCode = 0;
        if (syncExecutor.isNull()) {
            qDebug() << "  - Ipg sync interface non e' stato inizializzato";
            return false;
        }

        GetLaserStatusInput input;
        input.setCommand(COMMAND_GET_LASER_STATUS);
        if (syncExecutor->execute<GetLaserStatusInput, GetLaserStatusOutput, GetLaserStatusMarshaller>(input, laserStatus) > 0)
            return false;

        executionCode = laserStatus.getExecutionCode();
        return true;

    }

    bool getIsConnected() const { return isConnected; }

    void setIsConnected(bool value) { isConnected = value; }

};


}

#endif // IPG_SYNC_INTERFACE_HPP
