#ifndef SET_FREQUENCY_MARSHALLER_HPP
#define SET_FREQUENCY_MARSHALLER_HPP

#include "../beans/SetFrequency.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class SetFrequencyMarshaller : public IpgMarshaller<SetFrequencyInput, SetFrequencyOutput> {
public:
    typedef SetFrequencyMarshaller* Ptr;
    typedef const SetFrequencyMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetFrequencyInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream << input->getFrequency();

//        bytes.append(input->getFrequency());

        return true;

    }

    virtual bool unmarshallData(const QByteArray& bytes, SetFrequencyOutput::Ptr output) {

//        const int offsets[2] = {0, 2};

        IPG_USHORT result;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> result;

        output->setResult(result);

        /*
        bool ok;

        result = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        output->setResult(result);
        */

        return true;

    }

};

}

#endif // SET_FREQUENCY_MARSHALLER_HPP
