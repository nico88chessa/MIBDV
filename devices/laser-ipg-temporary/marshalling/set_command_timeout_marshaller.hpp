#ifndef IPG_LASER_TEMP_SET_COMMAND_TIMEOUT_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_COMMAND_TIMEOUT_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_command_timeout.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>


namespace ipg {

class SetCommandTimeoutMarshaller : public IpgMarshaller<SetCommandTimeoutInput, SetCommandTimeoutOutput> {
public:
    typedef SetCommandTimeoutMarshaller* Ptr;
    typedef const SetCommandTimeoutMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetCommandTimeoutInput::Ptr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getTimeout();

        /************************************
        bytes.append(input->getTimeout());
        ************************************/

        return true;

    }

};

}

#endif // SET_COMMAND_TIMEOUT_MARSHALLER_HPP
