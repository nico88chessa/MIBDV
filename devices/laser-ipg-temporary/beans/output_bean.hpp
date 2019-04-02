#ifndef IPG_LASER_TEMP_OUTPUT_BEAN_HPP
#define IPG_LASER_TEMP_OUTPUT_BEAN_HPP

#include <laser-ipg-temporary/utility/datatype.hpp>

namespace ipg {

class OutputBean {

public:
    typedef OutputBean* Ptr;
    typedef const OutputBean* ConstPtr;

private:
    IPG_USHORT answer;
    IPG_USHORT executionCode;
    IPG_ULONG datasize;
    IPG_USHORT crc16;

public:

    OutputBean() : answer(0), executionCode(0), datasize(0), crc16(0) { }

    virtual ~OutputBean() { }

    IPG_ULONG getDatasize() const { return datasize; }

    void setDatasize(const IPG_ULONG& value) { datasize = value; }

    IPG_USHORT getExecutionCode() const { return executionCode; }

    void setExecutionCode(const IPG_USHORT& value) { executionCode = value; }

    IPG_USHORT getCrc16() const { return crc16; }

    void setCrc16(const IPG_USHORT& value) { crc16 = value; }

    IPG_USHORT getAnswer() const { return answer; }

    void setAnswer(const IPG_USHORT& value) { answer = value; }

};

}

Q_DECLARE_METATYPE(ipg::OutputBean)

#endif // OUTPUT_BEAN_HPP
