#ifndef IPG_MARSHALLER_HPP
#define IPG_MARSHALLER_HPP

#include <communication/ethernet/AbstractMarshaller.hpp>
#include "../beans/InputBean.hpp"
#include "../beans/OutputBean.hpp"
#include "../utility/Utils.hpp"
#include "../utility/TypeTraits.hpp"

namespace ipg {

namespace ce = communication::ethernet;

template <typename I, typename O>
class IpgMarshaller : public ce::AbstractMarshaller<I,O> {

protected:

    virtual bool marshallData(const I* input, QByteArray& bytes) {

        Q_UNUSED(input)
        Q_UNUSED(bytes)
        return true;

    }

    virtual bool unmarshallData(const QByteArray& bytes, O* output) {

        Q_UNUSED(bytes)
        Q_UNUSED(output)
        return true;

    }

    virtual bool marshall(const I* input, QByteArray& bytes) override {

        bytes.clear();

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        // TODO NIC 27/02/2019 - rimettere static_cast se possibile
        InputBean::ConstPtr inputBean = reinterpret_cast<InputBean::ConstPtr>(input);

        stream << inputBean->getCommand();
        stream << inputBean->getReserved();
        stream << inputBean->getDatasize();

        // tengo distinte le parti non associate all'header per il marshal dei dati
        QByteArray dataBytes;

        if (inputBean->getDatasize() > 0)
            if (!this->marshallData(input, dataBytes))
                return false;

        stream.writeRawData(dataBytes.constData(), dataBytes.size());

//        qDebug() << "marshall before crc16 byte size: " << bytes.size();
//        qDebug() << "marshall before crc16 bytes: " << bytes;

        const unsigned char* bytes4Crc16 = reinterpret_cast<const unsigned char*>(bytes.constData());
        if (bytes4Crc16 == nullptr)
            return false;

        IPG_USHORT crc16 = Utils::getCrc16(bytes4Crc16, bytes.size());
        stream << crc16;

//        qDebug() << "marshall byte size: " << bytes.size();
//        qDebug() << "marshall bytes: " << bytes;
//        qDebug() << "marshall crc16: " << crc16;

        return true;

    }

    virtual bool unmarshall(const QByteArray& bytes, O* output) override {

        // offset dei singoli campi
        const int offsets[4] = {0, 2, 4, 8};

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        // TODO NIC 27/02/2019 - rimettere static_cast se possibile
        OutputBean::Ptr outputBean = reinterpret_cast<OutputBean::Ptr>(output);

        IPG_USHORT answer;
        IPG_USHORT executionCode;
        IPG_ULONG dataSize;

        stream >> answer;
        stream >> executionCode;
        stream >> dataSize;

        IPG_USHORT crc16Bytes;
        QDataStream streamCrc16(bytes.mid(offsets[3]+dataSize, 2));
        streamCrc16.setByteOrder(QDataStream::LittleEndian);
        streamCrc16 >> crc16Bytes;
        outputBean->setCrc16(crc16Bytes);

//        qDebug() << "unmarshall crc16: " << outputBean->getCrc16();
//        qDebug() << "unmarshall bytes: " << bytes;
//        qDebug() << "unmarshall byte size: " << bytes.size();

        // controllo che il crc16 sia lo stesso fra inviato e calcolato
        const QByteArray& data = bytes.mid(0, bytes.size()-2);
        if (crc16Bytes != Utils::getCrc16(reinterpret_cast<const unsigned char*>(data.constData()), data.size())) {
            qWarning() << "  - Errore nel controllo codice CRC16 in IpgMarshaller.";
            return false;
        }

        outputBean->setAnswer(answer);
        outputBean->setExecutionCode(executionCode);
        outputBean->setDatasize(dataSize);

//        if (executionCode > 0)
//            return false;

        if (dataSize > 0)
            // per l'unmarshalling dei dati, passo solo i byte dei dati (senza header)
            if (!this->unmarshallData(bytes.mid(offsets[3], dataSize), output))
                return false;

        return true;

    }

};

}

#endif // IPG_MARSHALLER_HPP
