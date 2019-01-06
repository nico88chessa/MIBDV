#ifndef SET_MODE_INDEX_MARSHALLER_HPP
#define SET_MODE_INDEX_MARSHALLER_HPP

#include "../beans/SetModeIndex.hpp"
#include "../marshalling/IpgMarshaller.hpp"

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
