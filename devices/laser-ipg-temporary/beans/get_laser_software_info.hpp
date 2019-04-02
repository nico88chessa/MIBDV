#ifndef IPG_LASER_TEMP_GET_LASER_SOFTWARE_INFO_HPP
#define IPG_LASER_TEMP_GET_LASER_SOFTWARE_INFO_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

typedef InputBean GetLaserSoftwareInfoInput;

class GetLaserSoftwareInfoOutput : public OutputBean {
public:
    typedef GetLaserSoftwareInfoOutput* Ptr;
    typedef const GetLaserSoftwareInfoOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_USHORT UIConfigLen;
    QString UIConfig;
    IPG_USHORT laserConfigLen;
    QString laserConfig;

public:
    IPG_USHORT getUIConfigLen() const { return UIConfigLen; }

    void setUIConfigLen(const IPG_USHORT& value) { UIConfigLen = value; }

    QString getUIConfig() const { return UIConfig; }

    void setUIConfig(const QString& value) { UIConfig = value; }

    IPG_USHORT getLaserConfigLen() const { return laserConfigLen; }

    void setLaserConfigLen(const IPG_USHORT& value) { laserConfigLen = value; }

    QString getLaserConfig() const { return laserConfig; }

    void setLaserConfig(const QString& value) { laserConfig = value; }

    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }
};

}

Q_DECLARE_METATYPE(ipg::GetLaserSoftwareInfoOutput)

#endif // GET_LASER_SOFTWARE_INFO_HPP
