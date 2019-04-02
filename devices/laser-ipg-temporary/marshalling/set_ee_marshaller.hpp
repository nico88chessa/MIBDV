#ifndef IPG_LASER_TEMP_SET_EE_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_EE_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_ee.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class SetEEMarshaller : public IpgMarshaller<SetEEInput, SetEEOutput> {
public:
    typedef SetEEMarshaller* Ptr;
    typedef const SetEEMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetEEInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getEmissionOn();

//        bytes.append(input->getEmissionOn());

        return true;

    }

};

}


#endif // SET_EE_MARSHALLER_HPP
