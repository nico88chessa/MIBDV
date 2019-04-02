#ifndef IPG_LASER_TEMP_SET_GL_MODE_MARSHALLER_HPP
#define IPG_LASER_TEMP_SET_GL_MODE_MARSHALLER_HPP

#include <laser-ipg-temporary/beans/set_gl_mode.hpp>
#include <laser-ipg-temporary/marshalling/ipg_marshaller.hpp>

namespace ipg {

class SetGLModeMarshaller : public IpgMarshaller<SetGLModeInput, SetGLModeOutput> {
public:
    typedef SetGLModeMarshaller* Ptr;
    typedef const SetGLModeMarshaller* ConstPtr;

protected:

    virtual bool marshallData(SetGLModeInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getGlMode();

//        bytes.append(input->getGlMode());

        return true;

    }

};

}

#endif // SET_GL_MODE_MARSHALLER_HPP
