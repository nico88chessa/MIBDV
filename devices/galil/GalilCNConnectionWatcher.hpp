#ifndef GALILCNCONNECTIONWATCHER_HPP
#define GALILCNCONNECTIONWATCHER_HPP

#include "AbstractDeviceConnectionWatcher.hpp"

namespace PROGRAM_NAMESPACE {

class GalilCNConnectionWatcher : public AbstractDeviceConnectionWatcher {
    Q_OBJECT

public:
    using Ptr = GalilCNConnectionWatcher*;
    using ConstPtr = const GalilCNConnectionWatcher*;

private:
    QTimer timerKeepAlive;
public:
    explicit GalilCNConnectionWatcher(QObject* parent = nullptr);

    virtual ~GalilCNConnectionWatcher() override;

protected:
    virtual void reconnectDevice() override;

};

}

#endif // GALILCNCONNECTIONWATCHER_HPP
