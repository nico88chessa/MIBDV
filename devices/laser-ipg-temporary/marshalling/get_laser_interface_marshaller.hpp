#ifndef IPG_LASER_TEMP_GET_LASER_INTERFACE_MARSHALLER_HPP
#define IPG_LASER_TEMP_GET_LASER_INTERFACE_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/get_laser_interface.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class GetLaserInterfaceMarshaller : public IpgMarshaller<GetLaserInterfaceInput, GetLaserInterfaceOutput> {
public:
    typedef GetLaserInterfaceMarshaller* Ptr;
    typedef const GetLaserInterfaceMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetLaserInterfaceOutput::Ptr output) {

        /*
        const int offsets[13] = {
            0,
            2,
            3,
            4,
            6,
            8,
            10,
            12,
            14,
            16,
            18,
            20,
            22
        };
        */

        IPG_USHORT version;
        IPG_BYTE mode;
        IPG_BYTE reserved;
        IPG_MAIN_INTERFACE mainInterface;
        IPG_INTERFACE_CONFIG powerConfig;
        IPG_INTERFACE_CONFIG eeConfig;
        IPG_INTERFACE_CONFIG emConfig;
        IPG_INTERFACE_CONFIG glConfig;
        IPG_INTERFACE_CONFIG prrConfig;
        IPG_INTERFACE_CONFIG psConfig;
        IPG_INTERFACE_CONFIG resetConfig;
        IPG_INTERFACE_CONFIG miConfig;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> version;
        stream >> mode;
        stream >> reserved;
        stream >> mainInterface;
        stream >> powerConfig;
        stream >> eeConfig;
        stream >> emConfig;
        stream >> glConfig;
        stream >> prrConfig;
        stream >> psConfig;
        stream >> resetConfig;
        stream >> miConfig;

        output->setVersion(version);
        output->setMode(mode);
        output->setReserved(reserved);
        output->setMainInterface(mainInterface);
        output->setPowerConfig(powerConfig);
        output->setEeConfig(eeConfig);
        output->setEmConfig(emConfig);
        output->setGlConfig(glConfig);
        output->setPrrConfig(prrConfig);
        output->setPsConfig(psConfig);
        output->setResetConfig(resetConfig);
        output->setMiConfig(miConfig);

        /*
        bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        mode = bytes.at(offsets[1]);

        mainInterface = bytes.mid(offsets[3], offsets[4] - offsets[3]).toUShort(&ok);
        if (!ok) return false;

        powerConfig = bytes.mid(offsets[4], offsets[5] - offsets[4]).toUShort(&ok);
        if (!ok) return false;

        eeConfig = bytes.mid(offsets[5], offsets[6] - offsets[5]).toUShort(&ok);
        if (!ok) return false;

        emConfig = bytes.mid(offsets[6], offsets[7] - offsets[6]).toUShort(&ok);
        if (!ok) return false;

        glConfig = bytes.mid(offsets[7], offsets[8] - offsets[7]).toUShort(&ok);
        if (!ok) return false;

        prrConfig = bytes.mid(offsets[8], offsets[9] - offsets[8]).toUShort(&ok);
        if (!ok) return false;

        psConfig = bytes.mid(offsets[9], offsets[10] - offsets[9]).toUShort(&ok);
        if (!ok) return false;

        resetConfig = bytes.mid(offsets[10], offsets[11] - offsets[10]).toUShort(&ok);
        if (!ok) return false;

        miConfig = bytes.mid(offsets[11], offsets[12] - offsets[11]).toUShort(&ok);
        if (!ok) return false;

        output->setVersion(version);
        output->setMode(mode);
        output->setMainInterface(mainInterface);
        output->setPowerConfig(powerConfig);
        output->setEeConfig(eeConfig);
        output->setEmConfig(emConfig);
        output->setGlConfig(glConfig);
        output->setPrrConfig(prrConfig);
        output->setPsConfig(psConfig);
        output->setResetConfig(resetConfig);
        output->setMiConfig(miConfig);
        */
        return true;


    }

};

}

#endif // GET_LASER_INTERFACE_MARSHALLER_HPP
