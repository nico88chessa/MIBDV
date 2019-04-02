#ifndef IPG_LASER_TEMP_INPUT_BEAN_HPP
#define IPG_LASER_TEMP_INPUT_BEAN_HPP

#include <laser-ipg-temporary/utility/datatype.hpp>

namespace ipg {

class InputBean {

public:
    typedef InputBean* Ptr;
    typedef const InputBean* ConstPtr;

private:
    IPG_USHORT command;
    const IPG_USHORT reserved;
    IPG_ULONG datasize;
    IPG_USHORT crc16;

public:

    InputBean() : reserved(0), datasize(0) { }

    InputBean(const IPG_ULONG size) : reserved(0), datasize(size) { }

    IPG_USHORT getCommand() const { return command; }

    void setCommand(const IPG_USHORT& value) { command = value; }

    IPG_USHORT getReserved() const { return reserved; }

    IPG_ULONG getDatasize() const { return datasize; }

//    void setDatasize(const IPG_ULONG& value) { datasize = value; }

    IPG_USHORT getCrc16() const { return crc16; }

    void setCrc16(const IPG_USHORT& value) { crc16 = value; }
};


}

Q_DECLARE_METATYPE(ipg::InputBean)

#endif // INPUT_BEAN_HPP
