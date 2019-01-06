#ifndef SET_IP_SETTINGS_MARSHALLER_HPP
#define SET_IP_SETTINGS_MARSHALLER_HPP

#include "../beans/SetIpSettings.hpp"
#include "../marshalling/IpgMarshaller.hpp"

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
