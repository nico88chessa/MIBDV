#ifndef GET_CRITICAL_ERROR_COUNTER_MARSHALLER_HPP
#define GET_CRITICAL_ERROR_COUNTER_MARSHALLER_HPP

#include "../beans/GetCriticalErrorCounter.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class GetCriticalErrorCounterMarshaller : public IpgMarshaller<GetCriticalErrorCounterInput, GetCriticalErrorCounterOutput> {
public:
    typedef GetCriticalErrorCounterMarshaller* Ptr;
    typedef GetCriticalErrorCounterMarshaller* ConstPtr;

protected:

    virtual bool unmarshallData(const QByteArray& bytes, GetCriticalErrorCounterOutput::Ptr output) {

        // offset dei singoli campi
//        const int offsets[3] = {0, 2, 6};

        IPG_USHORT version;
        IPG_ULONG criticalErrorCounter;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream >> version;
        stream >> criticalErrorCounter;

        /*****************************************************
        bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        criticalErrorCounter = bytes.mid(offsets[1], offsets[2] - offsets[1]).toULong(&ok);
        if (!ok) return false;
        *****************************************************/

        output->setVersion(version);
        output->setCriticalErrorsCounter(criticalErrorCounter);

        return true;

    }

};

}

#endif // GET_CRITICAL_ERROR_COUNTER_MARSHALLER_HPP
