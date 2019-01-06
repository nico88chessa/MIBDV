#ifndef SET_GL_MODE_HPP
#define SET_GL_MODE_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

class SetGLModeInput : public InputBean {
public:
    typedef SetGLModeInput* Ptr;
    typedef const SetGLModeInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 2;

private:
    IPG_USHORT glMode;

public:
    SetGLModeInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_USHORT getGlMode() const { return glMode; }

    void setGlMode(const IPG_USHORT& value) { glMode = value; }
};

typedef OutputBean SetGLModeOutput;


}

Q_DECLARE_METATYPE(ipg::SetGLModeInput)

#endif // SET_GL_MODE_HPP
