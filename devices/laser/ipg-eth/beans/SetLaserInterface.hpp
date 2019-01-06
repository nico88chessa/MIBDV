#ifndef SET_LASER_INTERFACE_HPP
#define SET_LASER_INTERFACE_HPP

#include "InputBean.hpp"
#include "OutputBean.hpp"

namespace ipg {

class SetLaserInterfaceInput : public InputBean {
public:
    typedef SetLaserInterfaceInput* Ptr;
    typedef const SetLaserInterfaceInput* ConstPtr;

private:
    static const IPG_ULONG INPUT_DATA_SIZE = 22;

private:
    IPG_USHORT version;
    IPG_BYTE mode;
    IPG_BYTE reserved;
    IPG_MAIN_INTERFACE mainInterface;
    IPG_INTERFACE_CONFIG powerConfig;
    IPG_INTERFACE_CONFIG eeConfig;
    IPG_INTERFACE_CONFIG emConfig;
    IPG_INTERFACE_CONFIG glConfig;
    IPG_INTERFACE_CONFIG prrConfig;
    IPG_INTERFACE_CONFIG psConfig;
    IPG_INTERFACE_CONFIG resetConfig;
    IPG_INTERFACE_CONFIG miConfig;

public:
    SetLaserInterfaceInput() : InputBean(INPUT_DATA_SIZE) { }

    IPG_BYTE getMode() const { return mode; }

    void setMode(const IPG_BYTE& value) { mode = value; }

    IPG_BYTE getReserved() const { return reserved; }

    IPG_MAIN_INTERFACE getMainInterface() const { return mainInterface; }

    void setMainInterface(const IPG_MAIN_INTERFACE& value) { mainInterface = value; }

    IPG_INTERFACE_CONFIG getPowerConfig() const { return powerConfig; }

    void setPowerConfig(const IPG_INTERFACE_CONFIG& value) { powerConfig = value; }

    IPG_INTERFACE_CONFIG getEeConfig() const { return eeConfig; }

    void setEeConfig(const IPG_INTERFACE_CONFIG& value) { eeConfig = value; }

    IPG_INTERFACE_CONFIG getEmConfig() const { return emConfig; }

    void setEmConfig(const IPG_INTERFACE_CONFIG& value) { emConfig = value; }

    IPG_INTERFACE_CONFIG getGlConfig() const { return glConfig; }

    void setGlConfig(const IPG_INTERFACE_CONFIG& value) { glConfig = value; }

    IPG_INTERFACE_CONFIG getPrrConfig() const { return prrConfig; }

    void setPrrConfig(const IPG_INTERFACE_CONFIG& value) { prrConfig = value; }

    IPG_INTERFACE_CONFIG getPsConfig() const { return psConfig; }

    void setPsConfig(const IPG_INTERFACE_CONFIG& value) { psConfig = value; }

    IPG_INTERFACE_CONFIG getResetConfig() const { return resetConfig; }

    void setResetConfig(const IPG_INTERFACE_CONFIG& value) { resetConfig = value; }

    IPG_INTERFACE_CONFIG getMiConfig() const { return miConfig; }

    void setMiConfig(const IPG_INTERFACE_CONFIG& value) { miConfig = value; }

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

};

class SetLaserInterfaceOutput : public OutputBean {
public:
    typedef SetLaserInterfaceOutput* Ptr;
    typedef const SetLaserInterfaceOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_USHORT status;

public:

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    IPG_USHORT getStatus() const { return status; }

    void setStatus(const IPG_USHORT& value) { status = value; }

};


}

Q_DECLARE_METATYPE(ipg::SetLaserInterfaceInput)
Q_DECLARE_METATYPE(ipg::SetLaserInterfaceOutput)

#endif // SET_LASER_INTERFACE_HPP
