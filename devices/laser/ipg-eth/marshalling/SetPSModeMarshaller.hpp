#ifndef SET_PS_MODE_MARSHALLER_HPP
#define SET_PS_MODE_MARSHALLER_HPP

#include "../beans/SetPSMode.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class SetPSModeMarshaller : public IpgMarshaller<SetPSModeInput, SetPSModeOutput> {
public:
    typedef SetPSModeMarshaller* Ptr;
    typedef const SetPSModeMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetPSModeInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getPowerSupply();

//        bytes.append(input->getPowerSupply());

        return true;

    }

};

}

#endif // SET_PS_MODE_MARSHALLER_HPP
