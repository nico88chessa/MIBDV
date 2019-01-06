#ifndef SET_EE_MARSHALLER_HPP
#define SET_EE_MARSHALLER_HPP

#include "../beans/SetEE.hpp"
#include "../marshalling/IpgMarshaller.hpp"

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
