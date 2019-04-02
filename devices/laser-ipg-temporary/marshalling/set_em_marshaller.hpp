#ifndef IPG_LASER_TEMP_SET_EM_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_EM_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_em.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class SetEMMarshaller : public IpgMarshaller<SetEMInput, SetEMOutput> {
public:
     typedef SetEMMarshaller* Ptr;
     typedef const SetEMMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetEMInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getModulationOn();

//        bytes.append(input->getModulationOn());

        return true;

    }

};

}

#endif // SET_EM_MARSHALLER_HPP
