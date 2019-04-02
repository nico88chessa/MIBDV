#ifndef IPG_LASER_TEMP_GET_NETWORK_SETTINGS_HPP
#define IPG_LASER_TEMP_GET_NETWORK_SETTINGS_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

typedef InputBean GetNetworkSettingsInput;

class GetNetworkSettingsOutput : public OutputBean {
public:
    typedef GetNetworkSettingsOutput* Ptr;
    typedef const GetNetworkSettingsOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_USHORT config;
    IPG_ULONG ipAddress;
    IPG_ULONG ipMask;
    IPG_ULONG defaultGateway;
    IPG_USHORT port;
    QByteArray mac;
    IPG_ULONG commandTimeout;

public:
    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    IPG_USHORT getConfig() const { return config; }

    void setConfig(const IPG_USHORT& value) { config = value; }

    IPG_ULONG getIpAddress() const { return ipAddress; }

    void setIpAddress(const IPG_ULONG& value) { ipAddress = value; }

    IPG_ULONG getIpMask() const { return ipMask; }

    void setIpMask(const IPG_ULONG& value) { ipMask = value; }

    IPG_ULONG getDefaultGateway() const { return defaultGateway; }

    void setDefaultGateway(const IPG_ULONG& value) { defaultGateway = value; }

    IPG_USHORT getPort() const { return port; }

    void setPort(const IPG_USHORT& value) { port = value; }

    QByteArray getMac() const { return mac; }

    void setMac(const QByteArray& value) { mac = value; }

    IPG_ULONG getCommandTimeout() const { return commandTimeout; }

    void setCommandTimeout(const IPG_ULONG& value) { commandTimeout = value; }

};


}

Q_DECLARE_METATYPE(ipg::GetNetworkSettingsOutput)

#endif // GET_NETWORK_SETTINGS_HPP
