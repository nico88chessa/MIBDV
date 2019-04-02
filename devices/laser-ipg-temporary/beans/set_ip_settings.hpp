#ifndef IPG_LASER_TEMP_SET_IP_SETTINGS_HPP
#define IPG_LASER_TEMP_SET_IP_SETTINGS_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

class SetIpSettingsInput : public InputBean {
public:
    typedef SetIpSettingsInput* Ptr;
    typedef const SetIpSettingsInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 18;

private:
    IPG_USHORT version;
    IPG_USHORT config;
    IPG_ULONG ipAddress;
    IPG_ULONG ipMask;
    IPG_ULONG defaultGateway;
    IPG_USHORT port;

public:
    SetIpSettingsInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getPort() const { return port; }

    void setPort(const IPG_USHORT& value) { port = value; }

    IPG_ULONG getDefaultGateway() const { return defaultGateway; }

    void setDefaultGateway(const IPG_ULONG& value) { defaultGateway = value; }

    IPG_ULONG getIpMask() const { return ipMask; }

    void setIpMask(const IPG_ULONG& value) { ipMask = value; }

    IPG_ULONG getIpAddress() const { return ipAddress; }

    void setIpAddress(const IPG_ULONG& value) { ipAddress = value; }

    IPG_USHORT getConfig() const { return config; }

    void setConfig(const IPG_USHORT& value) { config = value; }

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

};

typedef OutputBean SetIpSettingsOutput;


}

Q_DECLARE_METATYPE(ipg::SetIpSettingsInput)

#endif // SET_IP_SETTINGS_HPP
