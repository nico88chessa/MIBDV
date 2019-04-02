#ifndef IPG_LASER_TEMP_MARSHALLER_INTERFACE_HPP
#define IPG_LASER_TEMP_MARSHALLER_INTERFACE_HPP

#include <QByteArray>

namespace ipg {

class MarshallerInterface {

public:
    typedef MarshallerInterface* Ptr;
    typedef const MarshallerInterface* ConstPtr;

public:

    virtual ~MarshallerInterface() { }

    virtual bool marshall(const void* i, QByteArray& b) = 0;

    virtual bool unmarshall(const QByteArray& b, void* o) = 0;

};

}

#endif // MARSHALLER_INTERFACE_HPP
