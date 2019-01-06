#ifndef GET_LASER_SOFTWARE_INFO_MARSHALLER_HPP
#define GET_LASER_SOFTWARE_INFO_MARSHALLER_HPP

#include "../beans/GetLaserSoftwareInfo.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class GetLaserSoftwareInfoMarshaller : public IpgMarshaller<GetLaserSoftwareInfoInput, GetLaserSoftwareInfoOutput> {
public:
    typedef GetLaserSoftwareInfoMarshaller* Ptr;
    typedef const GetLaserSoftwareInfoMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetLaserSoftwareInfoOutput::Ptr output) {

//        const int offsets[3] = {0, 2, 4};

        IPG_USHORT version;
        IPG_USHORT uiConfigLen;
        QString uiConfig;
        IPG_USHORT laserConfigLen;
        QString laserConfig;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> version;
        stream >> uiConfigLen;

        char* uiConfigBuffer = new char[uiConfigLen];
        stream.readRawData(uiConfigBuffer, uiConfigLen);
        uiConfig.append(uiConfigBuffer);

        stream >> laserConfigLen;

        char* laserConfigBuffer = new char[laserConfigLen];
        stream.readRawData(laserConfigBuffer, laserConfigLen);
        laserConfig.append(laserConfigBuffer);

        output->setVersion(version);
        output->setUIConfigLen(uiConfigLen);
        output->setUIConfig(uiConfig);
        output->setLaserConfigLen(laserConfigLen);
        output->setLaserConfig(laserConfig);


        /*
        bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        uiConfigLen = bytes.mid(offsets[1], offsets[2] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        uiConfig < bytes.mid(offsets[2], uiConfigLen);

        laserConfigLen = bytes.mid(offsets[2] + uiConfigLen, 2).toUShort(&ok);
        if (!ok) return false;

        laserConfig < bytes.mid(offsets[2] + uiConfigLen + 2, laserConfigLen);


        output->setVersion(version);
        output->setUIConfigLen(uiConfigLen);
        output->setUIConfig(uiConfig);
        output->setLaserConfigLen(laserConfigLen);
        output->setLaserConfig(laserConfig);
        */
        delete[] uiConfigBuffer;
        delete[] laserConfigBuffer;

        return true;

    }

};

}

#endif // GET_LASER_SOFTWARE_INFO_MARSHALLER_HPP
