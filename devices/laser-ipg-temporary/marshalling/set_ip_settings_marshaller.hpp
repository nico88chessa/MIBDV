#ifndef IPG_LASER_TEMP_SET_IP_SETTINGS_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_IP_SETTINGS_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_ip_settings.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>


namespace ipg {

class SetIpSettingsMarshaller : public IpgMarshaller<SetIpSettingsInput, SetIpSettingsOutput> {
public:
    typedef SetIpSettingsMarshaller* Ptr;
    typedef const SetIpSettingsMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetIpSettingsInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getVersion();
        stream << input->getConfig();
        stream << input->getIpAddress();
        stream << input->getIpMask();
        stream << input->getDefaultGateway();
        stream << input->getPort();

        /*********************************************
        bytes.append(input->getVersion());
        bytes.append(input->getConfig());
        bytes.append(input->getIpAddress());
        bytes.append(input->getIpMask());
        bytes.append(input->getDefaultGateway());
        bytes.append(input->getPort());
        *********************************************/

        return true;

    }

};

}

#endif // SET_IP_SETTINGS_MARSHALLER_HPP
