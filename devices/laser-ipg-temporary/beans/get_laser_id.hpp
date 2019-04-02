#ifndef IPG_LASER_TEMP_LASER_ID_HPP
#define IPG_LASER_TEMP_LASER_ID_HPP

#include <laser-ipg-temporary/beans/input_bean.hpp>
#include <laser-ipg-temporary/beans/output_bean.hpp>

namespace ipg {

typedef InputBean GetLaserIdInput;

class GetLaserIdOutput : public OutputBean {
public:
    typedef GetLaserIdOutput* Ptr;
    typedef const GetLaserIdOutput* ConstPtr;

private:
    IPG_USHORT version;
    IPG_SHORT laserProductLine;
    QString laserId;
    QString laserSN;

public:
    IPG_USHORT getVersion() const { return version; }

    void setVersion(const IPG_USHORT& value) { version = value; }

    IPG_SHORT getLaserProductLine() const { return laserProductLine; }

    void setLaserProductLine(const IPG_SHORT& value) { laserProductLine = value; }

    QString getLaserSN() const { return laserSN; }

    void setLaserSN(const QString& value) { laserSN = value; }

    QString getLaserId() const { return laserId; }

    void setLaserId(const QString& value) { laserId = value; }
};


}

Q_DECLARE_METATYPE(ipg::GetLaserIdOutput)

#endif // LASER_ID_HPP
