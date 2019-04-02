#ifndef IPG_LASER_TEMP_IPG_MARSHALLER_HPP
#define IPG_LASER_TEMP_IPG_MARSHALLER_HPP

#include <Logger.hpp>

#include <laser-ipg-temporary/marshalling/marshaller_interface.hpp>
#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>
#include <laser-ipg-temporary/utility/utils.hpp>

namespace ipg {

template <typename I, typename O>
class IpgMarshaller : public MarshallerInterface {

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

    bool marshall(const I* input, QByteArray& bytes) {

        bytes.clear();

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        InputBean::ConstPtr inputBean = static_cast<InputBean::ConstPtr>(input);

        stream << inputBean->getCommand();
        stream << inputBean->getReserved();
        stream << inputBean->getDatasize();

        // tengo distinte le parti non associate all'header per il marshal dei dati
        QByteArray dataBytes;

        if (inputBean->getDatasize() > 0)
            if (!this->marshallData(input, dataBytes))
                return false;

        stream.writeRawData(dataBytes.constData(), dataBytes.size());

        const unsigned char* bytes4Crc16 = reinterpret_cast<const unsigned char*>(bytes.constData());
        if (bytes4Crc16 == NULL)
            return false;

        IPG_USHORT crc16 = getCrc16(bytes4Crc16, bytes.size());
        stream << crc16;

        return true;

    }

    bool unmarshall(const QByteArray& bytes, O* output) {

        using namespace PROGRAM_NAMESPACE;
        // offset dei singoli campi
        const int offsets[4] = {0, 2, 4, 8};

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        OutputBean::Ptr outputBean = static_cast<OutputBean::Ptr>(output);

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

        // controllo che il crc16 sia lo stesso fra inviato e calcolato
        const QByteArray& data = bytes.mid(0, bytes.size()-2);
        if (crc16Bytes != getCrc16(reinterpret_cast<const unsigned char*>(data.constData()), data.size())) {
            traceWarn() << "Errore nel controllo codice CRC16 in IpgMarshaller.";
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

public:

    bool marshall(const void* i, QByteArray& b) {

        const I* inputCast = static_cast<const I*>(i);
        return this->marshall(inputCast, b);

    }

    bool unmarshall(const QByteArray& b, void* o) {

        O* outputCast = static_cast<O*>(o);
        return this->unmarshall(b, outputCast);

    }

};



}

#endif // IPG_MARSHALLER_HPP
