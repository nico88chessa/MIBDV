#ifndef IPG_LASER_TEMP_RESET_CRITICAL_ERROR_MARSHALLER_HPP
#define IPG_LASER_TEMP_RESET_CRITICAL_ERROR_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/reset_critical_error.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class ResetCriticalErrorMarshaller : public IpgMarshaller<ResetCriticalErrorInput, ResetCriticalErrorOutput> {
public:
    typedef ResetCriticalErrorMarshaller* Ptr;
    typedef const ResetCriticalErrorMarshaller* ConstPtr;

protected:

    virtual bool marshallData(ResetCriticalErrorInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        int size = input->getDatasize();
        char* data = new char[size];
        memset(data, '\0', size);
        strcpy(data, input->getData().toStdString().data());

        stream.writeRawData(data, size);

        delete[] data;

//        bytes.append(input->getData());

        return true;
    }

};

}


#endif // RESET_CRITICAL_ERROR_MARSHALLER_HPP
