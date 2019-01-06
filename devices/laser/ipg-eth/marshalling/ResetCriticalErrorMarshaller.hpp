#ifndef RESET_CRITICAL_ERROR_MARSHALLER_HPP
#define RESET_CRITICAL_ERROR_MARSHALLER_HPP

#include "../beans/ResetCriticalError.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class ResetCriticalErrorMarshaller : public IpgMarshaller<ResetCriticalErrorInput, ResetCriticalErrorOutput> {
public:
    typedef ResetCriticalErrorMarshaller* Ptr;
    typedef const ResetCriticalErrorMarshaller* ConstPtr;

protected:

    virtual bool marshallData(ResetCriticalErrorInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        IPG_ULONG size = input->getDatasize();
        char* data = new char[size];
        memset(data, '\0', size);
        strcpy(data, input->getData().toStdString().data());

        stream.writeRawData(data, size);

//        bytes.append(input->getData());

        delete[] data;

        return true;
    }

};

}


#endif // RESET_CRITICAL_ERROR_MARSHALLER_HPP
