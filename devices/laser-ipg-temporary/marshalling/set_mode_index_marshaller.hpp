#ifndef IPG_LASER_TEMP_SET_MODE_INDEX_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_MODE_INDEX_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_mode_index.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class SetModeIndexMarshaller : public IpgMarshaller<SetModeIndexInput, SetModeIndexOutput> {
public:
    typedef SetModeIndexMarshaller* Ptr;
    typedef const SetModeIndexMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetModeIndexInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getModeIndex();

//        bytes.append(input->getModeIndex());

        return true;

    }

};

}

#endif // SET_MODE_INDEX_MARSHALLER_HPP
