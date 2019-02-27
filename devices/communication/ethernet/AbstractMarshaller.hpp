#ifndef ABSTRACT_MARSHALLER_HPP
#define ABSTRACT_MARSHALLER_HPP

#include "MarshallerInterface.hpp"

namespace communication {

namespace ethernet {

template <typename I, typename O>
class AbstractMarshaller : public MarshallerInterface {

protected:

    virtual bool marshall(const I* input, QByteArray& b) = 0;

    virtual bool unmarshall(const QByteArray& b, O* output) = 0;

public:

    virtual bool marshall(const void* input, QByteArray& b) {
        const I* i = static_cast<const I*>(input);
        return this->marshall(i, b);
    }

    virtual bool unmarshall(const QByteArray& b, void* output) {
        O* o = static_cast<O*>(output);
        return this->unmarshall(b, o);
    }

    virtual void* instanceInput() override {
        return new I();
    }

    virtual void* instanceOutput() override {
        return new O();
    }

};


}

}

#endif // MARSHALLER_INTERFACE_HPP
