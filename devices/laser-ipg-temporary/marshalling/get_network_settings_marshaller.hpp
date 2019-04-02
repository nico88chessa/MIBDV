#ifndef IPG_LASER_TEMP_GET_NETWORK_SETTINGS_MARSHALLER_HPP
#define IPG_LASER_TEMP_GET_NETWORK_SETTINGS_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/get_network_settings.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>




namespace ipg {

class GetNetworkSettingsMarshaller : public IpgMarshaller<GetNetworkSettingsInput, GetNetworkSettingsOutput> {
public:
    typedef GetNetworkSettingsMarshaller* Ptr;
    typedef const GetNetworkSettingsMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetNetworkSettingsOutput::Ptr output) {

        // offset dei singoli campi
        /*
        const int offsets[9] = {0, 2, 4, 8, 12, 16, 18, 24, 28};
        bool ok;
        */

        IPG_USHORT version;
        IPG_USHORT config;
        IPG_ULONG ipAddress;
        IPG_ULONG ipMask;
        IPG_ULONG defaultGateway;
        IPG_USHORT port;
        IPG_BYTE mac[6];
        IPG_ULONG commandTimeout;

        QDataStream outStream(bytes);
        outStream.setByteOrder(QDataStream::LittleEndian);

        outStream >> version;
        outStream >> config;
        outStream >> ipAddress;
        outStream >> ipMask;
        outStream >> defaultGateway;
        outStream >> port;

        outStream.readRawData(reinterpret_cast<char*>(mac), 6);

        outStream >> commandTimeout;

        /*
        version = bytes.mid(offsets[0], offsets[0+1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        config = bytes.mid(offsets[1], offsets[1+1] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        ipAddress = bytes.mid(offsets[2], offsets[2+1] - offsets[2]).toULong(&ok);
        if (!ok) return false;

        ipMask = bytes.mid(offsets[3], offsets[3+1] - offsets[3]).toULong(&ok);
        if (!ok) return false;

        defaultGateway = bytes.mid(offsets[4], offsets[4+1] - offsets[4]).toULong(&ok);
        if (!ok) return false;

        port = bytes.mid(offsets[5], offsets[5+1] - offsets[5]).toUShort(&ok);
        if (!ok) return false;

        mac = bytes.mid(offsets[6], offsets[6+1] - offsets[6]);

        commandTimeout = bytes.mid(offsets[7], offsets[7+1] - offsets[7]).toULong(&ok);
        if (!ok) return false;
        */

        output->setVersion(version);
        output->setConfig(config);
        output->setIpAddress(ipAddress);
        output->setIpMask(ipMask);
        output->setDefaultGateway(defaultGateway);
        output->setPort(port);
        output->setMac(QByteArray(reinterpret_cast<char*>(mac), 6));
        output->setCommandTimeout(commandTimeout);

        return true;

    }

};

}


#endif // GET_NETWORK_SETTINGS_MARSHALLER_HPP
