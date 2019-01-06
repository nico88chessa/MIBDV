#ifndef IPG_ASYNC_INTERFACE_HPP
#define IPG_ASYNC_INTERFACE_HPP

#include <QTcpSocket>
#include <QHostAddress>

#include "AbstractSender.hpp"
#include "AbstractReceiver.hpp"
#include "IpgAsyncExecutor.hpp"

namespace ipg {

class SenderImpl : public AbstractSender {
    Q_OBJECT

public:
    typedef SenderImpl* Ptr;
    typedef const SenderImpl* ConstPtr;

private:
    QTcpSocket* socket;

public:

    SenderImpl(QTcpSocket* _s, QObject* parent=0) :
        AbstractSender(parent),
        socket(_s) {
    }

public slots:

    virtual int sendData(const QByteArray& data) {

        int byteWritten = this->socket->write(data);

        if (byteWritten == -1)
            return 1;

        if (!socket->waitForBytesWritten(CONNECTION_MAX_WRITE_TIME_MS))
            return 2;

        return 0;
    }

};


class ReceiverImpl : public AbstractReceiver {
    Q_OBJECT

public:
    typedef ReceiverImpl* Ptr;
    typedef const ReceiverImpl* ConstPtr;

private:
    QTcpSocket* socket;

public:

    ReceiverImpl(QTcpSocket* _s, QObject* parent=0) : AbstractReceiver(parent), socket(_s) {

        connect(socket, SIGNAL(readyRead()), this, SIGNAL(dataArrives()));

    }

    virtual bool hasData() {

        return socket->bytesAvailable() > 0;

    }

    virtual int readBytes(QByteArray& bytes) {

        bytes = socket->readAll();
        return 0;

    }

    virtual int readBytes(QByteArray& bytes, int count) {

        bytes = socket->read(count);
        return 0;

    }

};


class IpgAsyncInterface : public QObject {
    Q_OBJECT

public:
    typedef IpgAsyncInterface* Ptr;
    typedef const IpgAsyncInterface* ConstPtr;

private:
    QTcpSocket* socket;
    AbstractSender::Ptr sender;
    AbstractReceiver::Ptr receiver;
    AbstractAsyncExecutor::Ptr asyncExecutor;

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

    void connectedSignal();
    void disconnectedSignal();
    void errorSignal(QAbstractSocket::SocketError socketError);

public slots:

    void connectedHandler() {
        qDebug() << "  - Ipg async interface: connessione avvenuta con successo.";
        emit connectedSignal();
    }

    void disconnectHandler() {
        qDebug() << "  - Ipg async interface: disconnessione avvenuta.";
        emit disconnectedSignal();
    }

    void errorHandler(QAbstractSocket::SocketError error) {
        qDebug() << "  - Ipg async interface: errore socket" << error;
        emit errorSignal(error);
    }

public:

    IpgAsyncInterface(QObject* parent=0) : QObject(parent) {

        socket = new QTcpSocket(parent);
        sender = new SenderImpl(socket, parent);
        receiver = new ReceiverImpl(socket, parent);
        asyncExecutor = new IpgAsyncExecutor(sender, receiver, parent);

        connect(asyncExecutor, SIGNAL(getNetworkSettingsSignal(ipg::GetNetworkSettingsOutput)), this, SIGNAL(getNetworkSettingsSignal(ipg::GetNetworkSettingsOutput)));
        connect(asyncExecutor, SIGNAL(setIpSettingsSignal(ipg::SetIpSettingsOutput)), this, SIGNAL(setIpSettingsSignal(ipg::SetIpSettingsOutput)));
        connect(asyncExecutor, SIGNAL(setCommandTimeoutSignal(ipg::SetCommandTimeoutOutput)), this, SIGNAL(setCommandTimeoutSignal(ipg::SetCommandTimeoutOutput)));
        connect(asyncExecutor, SIGNAL(getCriticalErrorCounterSignal(ipg::GetCriticalErrorCounterOutput)), this, SIGNAL(getCriticalErrorCounterSignal(ipg::GetCriticalErrorCounterOutput)));
        connect(asyncExecutor, SIGNAL(getLaserModelInfoSignal(ipg::GetLaserModelInfoOutput)), this, SIGNAL(getLaserModelInfoSignal(ipg::GetLaserModelInfoOutput)));
        connect(asyncExecutor, SIGNAL(getLaserSoftwareInfoSignal(ipg::GetLaserSoftwareInfoOutput)), this, SIGNAL(getLaserSoftwareInfoSignal(ipg::GetLaserSoftwareInfoOutput)));
        connect(asyncExecutor, SIGNAL(getModeParameterSignal(ipg::GetModeParameterOutput)), this, SIGNAL(getModeParameterSignal(ipg::GetModeParameterOutput)));
        connect(asyncExecutor, SIGNAL(getAlarmCounterSignal(ipg::GetAlarmCounterOutput)), this, SIGNAL(getAlarmCounterSignal(ipg::GetAlarmCounterOutput)));
        connect(asyncExecutor, SIGNAL(setLaserInterfaceSignal(ipg::SetLaserInterfaceOutput)), this, SIGNAL(setLaserInterfaceSignal(ipg::SetLaserInterfaceOutput)));
        connect(asyncExecutor, SIGNAL(getLaserInterfaceSignal(ipg::GetLaserInterfaceOutput)), this, SIGNAL(getLaserInterfaceSignal(ipg::GetLaserInterfaceOutput)));
        connect(asyncExecutor, SIGNAL(resetSignal(ipg::ResetOutput)), this, SIGNAL(resetSignal(ipg::ResetOutput)));
        connect(asyncExecutor, SIGNAL(resetCriticalErrorSignal(ipg::ResetCriticalErrorOutput)), this, SIGNAL(resetCriticalErrorSignal(ipg::ResetCriticalErrorOutput)));
        connect(asyncExecutor, SIGNAL(setModeIndexSignal(ipg::SetModeIndexOutput)), this, SIGNAL(setModeIndexSignal(ipg::SetModeIndexOutput)));
        connect(asyncExecutor, SIGNAL(setPSModeSignal(ipg::SetPSModeOutput)), this, SIGNAL(setPSModeSignal(ipg::SetPSModeOutput)));
        connect(asyncExecutor, SIGNAL(setGLModeSignal(ipg::SetGLModeOutput)), this, SIGNAL(setGLModeSignal(ipg::SetGLModeOutput)));
        connect(asyncExecutor, SIGNAL(setFrequencySignal(ipg::SetFrequencyOutput)), this, SIGNAL(setFrequencySignal(ipg::SetFrequencyOutput)));
        connect(asyncExecutor, SIGNAL(setPowerSignal(ipg::SetPowerOutput)), this, SIGNAL(setPowerSignal(ipg::SetPowerOutput)));
        connect(asyncExecutor, SIGNAL(setEESignal(ipg::SetEEOutput)), this, SIGNAL(setEESignal(ipg::SetEEOutput)));
        connect(asyncExecutor, SIGNAL(setEMSignal(ipg::SetEMOutput)), this, SIGNAL(setEMSignal(ipg::SetEMOutput)));
        connect(asyncExecutor, SIGNAL(getLaserStatusSignal(ipg::GetLaserStatusOutput)), this, SIGNAL(getLaserStatusSignal(ipg::GetLaserStatusOutput)));

        connect(this->socket, SIGNAL(connected()), this, SLOT(connectedHandler()));
        connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnectHandler()));
        connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorHandler(QAbstractSocket::SocketError)));

    }

    ~IpgAsyncInterface() {

        delete socket;
        delete sender;
        delete receiver;
        delete asyncExecutor;

    }

    bool connectToLaser(const QString& address, quint16 port) {

        socket->connectToHost(address, port);

        if (!socket->waitForConnected(CONNECTION_DEFAULT_TIMEOUT_MS)) {
            qDebug() << "  - Ipg async interface: connessione rifiutata.";
            return false;
        }

        return true;

    }

    void closeConnection() {

        socket->close();

    }

    bool getLaserId() {

        GetLaserIdInput input;
        input.setCommand(COMMAND_GET_LASERID);
        if (asyncExecutor->send<GetLaserIdInput, GetLaserIdMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getNetworkSettings() {

        GetNetworkSettingsInput input;
        input.setCommand(COMMAND_GET_NETWORK_SETTINGS);
        if (asyncExecutor->send<GetNetworkSettingsInput, GetNetworkSettingsMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool setIpSettings(const QString& ip, quint16 port, const QString& mask, const QString& gateway) {

        QHostAddress ipAddress(ip);
        IPG_ULONG ipAddressNumber = ipAddress.toIPv4Address();

        QHostAddress gatewayAddress(gateway);
        IPG_ULONG gatewayNumber = gatewayAddress.toIPv4Address();

        QHostAddress maskAddress(mask);
        IPG_ULONG maskNumber = maskAddress.toIPv4Address();

        // TODO IPG: verificare maschera configurazione;
        IPG_USHORT config = 0x03;
        config &= ~NETWORK_CONFIGURATION_MASK_DHCP_ENABLED;
        config &= ~NETWORK_CONFIGURATION_MASK_FTP_ENABLED;

        SetIpSettingsInput input;

        input.setCommand(COMMAND_SET_IPSETTINGS);
        input.setVersion(1);
        input.setIpAddress(ipAddressNumber);
        input.setDefaultGateway(gatewayNumber);
        input.setIpMask(maskNumber);
        input.setPort(port);
        input.setConfig(config);

        if (asyncExecutor->send<SetIpSettingsInput, SetIpSettingsMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool setCommandTimeout(quint16 timeoutMs) {

        SetCommandTimeoutInput input;

        input.setCommand(COMMAND_SET_COMMAND_TIMEOUT);
        input.setTimeout(timeoutMs);

        if (asyncExecutor->send<SetCommandTimeoutInput, SetCommandTimeoutMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getCriticalErrorCounter() {

        GetCriticalErrorCounterInput input;
        input.setCommand(COMMAND_GET_CRITICAL_ERROR_COUNTER);
        if (asyncExecutor->send<GetCriticalErrorCounterInput, GetCriticalErrorCounterMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getLaserModelInfo() {

        GetLaserModelInfoInput input;
        input.setCommand(COMMAND_GET_LASER_MODEL_INFO);
        if (asyncExecutor->send<GetLaserModelInfoInput, GetLaserModelInfoMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getLaserSoftwareInfo() {

        GetLaserSoftwareInfoInput input;

        input.setCommand(COMMAND_GET_LASER_SOFTWARE_INFO);
        if (asyncExecutor->send<GetLaserSoftwareInfoInput, GetLaserSoftwareInfoMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getModeParameter(quint16 indexMode) {

        GetModeParameterInput input;

        input.setCommand(COMMAND_GET_MODE_PARAMETER);
        input.setModeIndex(indexMode);

        if (asyncExecutor->send<GetModeParameterInput, GetModeParameterMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getAlarmCounter() {

        GetAlarmCounterInput input;
        input.setCommand(COMMAND_GET_ALARM_COUNTER);
        if (asyncExecutor->send<GetAlarmCounterInput, GetAlarmCounterMarshaller>(input) > 0)
            return false;
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
            quint16 miConfig
            ) {

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

        if (asyncExecutor->send<SetLaserInterfaceInput, SetLaserInterfaceMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool getLaserInterface() {

        GetLaserInterfaceInput input;

        input.setCommand(COMMAND_GET_LASER_INTERFACE);
        if (asyncExecutor->send<GetLaserInterfaceInput, GetLaserInterfaceMarshaller>(input) > 0)
            return false;

        return true;

    }

    bool reset() {

        ResetInput input;
        input.setCommand(COMMAND_RESET);
        if (asyncExecutor->send<ResetInput, ResetMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool resetCriticalError(const QString& ipgServiceString) {

        ResetCriticalErrorInput input;

        input.setCommand(COMMAND_RESET_CRITICAL_ERROR);
        input.setData(ipgServiceString);

        if (asyncExecutor->send<ResetCriticalErrorInput, ResetCriticalErrorMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool setModeIndex(quint16 mode) {

        SetModeIndexInput input;

        input.setCommand(COMMAND_SET_MODE_INDEX);
        input.setModeIndex(mode);

        if (asyncExecutor->send<SetModeIndexInput, SetModeIndexMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool setPsMode(bool powerSupply) {

        SetPSModeInput input;

        input.setCommand(COMMAND_SET_PS_MODE);
        input.setPowerSupply(powerSupply ? 1 : 0);

        if (asyncExecutor->send<SetPSModeInput, SetPSModeMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool setGlMode(bool guideLaser) {

        SetGLModeInput input;

        input.setCommand(COMMAND_SET_GL);
        input.setGlMode(guideLaser ? 1 : 0);

        if (asyncExecutor->send<SetGLModeInput, SetGLModeMarshaller>(input) > 0)
            return false;
        return true;

    }

    bool setFrequency(float frequency) {

        SetFrequencyInput input;

        input.setCommand(COMMAND_SET_FREQUENCY);
        input.setFrequency(frequency);

        if (asyncExecutor->send<SetFrequencyInput, SetFrequencyMarshaller>(input) > 0)
            return false;

        return true;

    }

    bool setPower(float power, bool& operationResult) {

        Q_UNUSED(operationResult)
        SetPowerInput input;

        input.setCommand(COMMAND_SET_POWER);
        input.setPower(power);

        if (asyncExecutor->send<SetPowerInput, SetPowerMarshaller>(input) > 0)
            return false;

        return true;

    }

    bool setEE(bool emission) {

        SetEEInput input;

        input.setCommand(COMMAND_SET_EE);
        input.setEmissionOn(emission ? 1 : 0);

        if (asyncExecutor->send<SetEEInput, SetEEMarshaller>(input) > 0)
            return false;

        return true;

    }

    bool setEM(bool modulation) {

        SetEMInput input;

        input.setCommand(COMMAND_SET_EM);
        input.setModulationOn(modulation ? 1 : 0);

        if (asyncExecutor->send<SetEMInput, SetEMMarshaller>(input) > 0)
            return false;

        return true;

    }

    bool getLaserStatus() {

        GetLaserStatusInput input;
        input.setCommand(COMMAND_GET_LASER_STATUS);
        if (asyncExecutor->send<GetLaserStatusInput, GetLaserStatusMarshaller>(input) > 0)
            return false;

        return true;

    }

};

}

#endif // IPG_ASYNC_INTERFACE_HPP
