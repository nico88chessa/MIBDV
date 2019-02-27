#ifndef MARSHALLER_INTERFACE_HPP
#define MARSHALLER_INTERFACE_HPP

#include <QByteArray>

namespace communication {

namespace ethernet {

class MarshallerInterface {

public:
    typedef MarshallerInterface* Ptr;
    typedef const MarshallerInterface* ConstPtr;

public:

    virtual ~MarshallerInterface() { }

    virtual bool marshall(const void* input, QByteArray& b) = 0;

    virtual bool unmarshall(const QByteArray& b, void* output) = 0;

    virtual void* instanceInput() = 0;

    virtual void* instanceOutput() = 0;

};

}

}

#endif // MARSHALLER_INTERFACE_HPP
