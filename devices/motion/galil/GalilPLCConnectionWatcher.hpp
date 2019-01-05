#ifndef GALILPLCCONNECTIONWATCHER_HPP
#define GALILPLCCONNECTIONWATCHER_HPP

#include <devices/AbstractDeviceConnectionWatcher.hpp>

namespace PROGRAM_NAMESPACE {

class GalilPLCConnectionWatcher : public AbstractDeviceConnectionWatcher {
    Q_OBJECT

public:
    using Ptr = GalilPLCConnectionWatcher*;
    using ConstPtr = const GalilPLCConnectionWatcher*;

private:
    QTimer timerKeepAlive;
public:
    explicit GalilPLCConnectionWatcher(QObject* parent = nullptr);

    virtual ~GalilPLCConnectionWatcher() override;

protected:
    virtual void reconnectDevice() override;

};

}

#endif // GALILPLCCONNECTIONWATCHER_HPP
