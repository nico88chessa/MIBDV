#ifndef LASER_ID_MARSHALLER_HPP
#define LASER_ID_MARSHALLER_HPP

#include "../beans/GetLaserId.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class GetLaserIdMarshaller : public IpgMarshaller<GetLaserIdInput, GetLaserIdOutput> {
public:
    typedef GetLaserIdMarshaller* Ptr;
    typedef const GetLaserIdMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetLaserIdOutput::Ptr output) {

        // offset dei singoli campi
        const int offsets[5] = {0, 2, 4, 68, 100};

        IPG_USHORT version;
        IPG_SHORT laserProductLine;
        QString laserID;
        QString laserSN;
        int size = 0;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> version;
        stream >> laserProductLine;

        size = offsets[3] - offsets[2];
        char* laserIdBuffer = new char[size];
        stream.readRawData(&laserIdBuffer[0], size);
        laserID.append(laserIdBuffer);

        size = offsets[4] - offsets[3];
        char* laserSNBuffer = new char[size];
        stream.readRawData(&laserSNBuffer[0], size);
        laserSN.append(laserSNBuffer);

        /*
        bool ok;

        laserSN.fromRawData(laserSNBuffer, laserSNSize);

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        laserProductLine = bytes.mid(offsets[1], offsets[2] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        laserID < bytes.mid(offsets[2], offsets[3] - offsets[2]);
        laserSN < bytes.mid(offsets[3], offsets[4] - offsets[3]);
        */

        output->setVersion(version);
        output->setLaserProductLine(laserProductLine);
        output->setLaserId(laserID);
        output->setLaserSN(laserSN);

        delete[] laserIdBuffer;
        delete[] laserSNBuffer;

        return true;

    }

};


}

#endif // LASER_ID_MARSHALLER_HPP
