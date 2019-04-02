#ifndef IPG_LASER_TEMP_SET_LASER_INTERFACE_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_LASER_INTERFACE_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_laser_interface.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class SetLaserInterfaceMarshaller : public IpgMarshaller<SetLaserInterfaceInput, SetLaserInterfaceOutput> {
public:
    typedef SetLaserInterfaceMarshaller* Ptr;
    typedef const SetLaserInterfaceMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetLaserInterfaceInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getVersion();
        stream << input->getMode();
        stream << input->getReserved();
        stream << input->getMainInterface();
        stream << input->getPowerConfig();
        stream << input->getEeConfig();
        stream << input->getEmConfig();
        stream << input->getGlConfig();
        stream << input->getPrrConfig();
        stream << input->getPsConfig();
        stream << input->getResetConfig();
        stream << input->getMiConfig();

        /*
        bytes.append(input->getVersion());
        bytes.append(input->getMode());
        bytes.append(input->getReserved());
        bytes.append(input->getMainInterface());
        bytes.append(input->getPowerConfig());
        bytes.append(input->getEeConfig());
        bytes.append(input->getEmConfig());
        bytes.append(input->getGlConfig());
        bytes.append(input->getPrrConfig());
        bytes.append(input->getPsConfig());
        bytes.append(input->getResetConfig());
        bytes.append(input->getMiConfig());
        */

        return true;

    }

    virtual bool unmarshallData(const QByteArray& bytes, SetLaserInterfaceOutput::Ptr output) {

//        const int offsets[3] = {0, 2, 4};

        IPG_USHORT version;
        IPG_USHORT status;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> version;
        stream >> status;

        output->setVersion(version);
        output->setStatus(status);

        /*
        bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        status = bytes.mid(offsets[1], offsets[2] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        output->setVersion(version);
        output->setStatus(status);
        */

        return true;

    }

};

}

#endif // SET_LASER_INTERFACE_MARSHALLER_HPP
