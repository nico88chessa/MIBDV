#ifndef SET_EM_MARSHALLER_HPP
#define SET_EM_MARSHALLER_HPP

#include "../beans/SetEM.hpp"
#include "../marshalling/IpgMarshaller.hpp"

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
