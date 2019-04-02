#ifndef IPG_LASER_TEMP_IPG_INSPECTOR_H
#define IPG_LASER_TEMP_IPG_INSPECTOR_H

#include <QObject>
#include <laser-ipg-temporary/communication/ipg_async_interface.hpp>

namespace ipg {

class IpgInspector : public QObject {
    Q_OBJECT

public:
    typedef IpgInspector* Ptr;
    typedef const IpgInspector* ConstPtr;

private:
    ipg::IpgAsyncInterface laserAsyncInterface;
    QTimer refreshTimer;
    QTimer standbyTimer;
    QString ip;
    quint16 port;

    static const int RECONNECTION_TIME_MS_DFLT = 10000;
    static const int REFRESH_TIME_MS_DFLT = 5000;
    static const int STANDBY_MAX_TIME_MS_DFLT = REFRESH_TIME_MS_DFLT * 5;

public:

    IpgInspector(QObject* parent=0);

    void setAddress(const QString& ip, quint16 port);

    void setRefreshTime(int refreshTime = REFRESH_TIME_MS_DFLT);

    void setStandbyTime(int standbyMaxTime = STANDBY_MAX_TIME_MS_DFLT);

    ~IpgInspector();

private slots:

    void process();

    void handleStatus(ipg::GetLaserStatusOutput);

    void handleDisconnection();

public slots:

    void restartProcess();

    void startProcess();

    void stopProcess();

signals:

    void connectedSignal();

    void disconnectedSignal();

    void laserStatusSignal(ipg::GetLaserStatusOutput);

    void processStartSignal();

    void processStopSignal();

};

}

#endif // IPG_INSPECTOR_H
