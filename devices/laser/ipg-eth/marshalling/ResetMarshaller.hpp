#ifndef RESET_MARSHALLER_HPP
#define RESET_MARSHALLER_HPP

#include "../beans/Reset.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class ResetMarshaller : public IpgMarshaller<ResetInput, ResetOutput> {
public:
    typedef ResetMarshaller* Ptr;
    typedef const ResetMarshaller* ConstPtr;

};

}

#endif // RESET_MARSHALLER_HPP
