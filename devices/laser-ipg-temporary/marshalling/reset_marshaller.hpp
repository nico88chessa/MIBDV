#ifndef IPG_LASER_TEMP_RESET_MARSHALLER_HPP
#define IPG_LASER_TEMP_RESET_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/reset.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class ResetMarshaller : public IpgMarshaller<ResetInput, ResetOutput> {
public:
    typedef ResetMarshaller* Ptr;
    typedef const ResetMarshaller* ConstPtr;

};

}

#endif // RESET_MARSHALLER_HPP
