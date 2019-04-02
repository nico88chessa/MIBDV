#ifndef IPG_LASER_TEMP_SET_PS_MODE_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_PS_MODE_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_ps_mode.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

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
