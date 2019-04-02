#ifndef IPG_LASER_TEMP_SET_POWER_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_POWER_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_power.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class SetPowerMarshaller : public IpgMarshaller<SetPowerInput, SetPowerOutput> {
public:
    typedef SetPowerMarshaller* Ptr;
    typedef const SetPowerMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetPowerInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream << input->getPower();

//        bytes.append(input->getPower());

        return true;

    }

protected:

    virtual bool unmarshallData(const QByteArray& bytes, SetPowerOutput::Ptr output) {

//        const int offsets[2] = {0, 2};

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        IPG_USHORT result;

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


#endif // SET_POWER_MARSHALLER_HPP
