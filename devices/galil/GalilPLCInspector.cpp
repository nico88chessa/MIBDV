#include "GalilPLCInspector.hpp"

#include <Settings.hpp>

using namespace PROGRAM_NAMESPACE;

GalilPLCInspector::GalilPLCInspector(QObject* parent) :
    AbstractConnectedDeviceInspector(parent) {

    Settings& s = Settings::instance();
    ipAddress = s.getGalilPLCIpAddress();

    device.reset(new GalilPLCController());
    this->setRefreshValue(s.getGalilPLCStatusRefreshIntervalMs());
    this->setRestartTime(s.getGalilPLCReconnectionIntervalMs());

}

bool GalilPLCInspector::getStatus(QVariant& status) {

    traceEnter;
    bool res;

    try {
        GalilPLCStatusBean s = this->getGalilPLCDevicePtr()->getStatus();
        status = QVariant::fromValue(s);
        res = true;
    } catch(NoStatusException& e) {
        Q_UNUSED(e)
        res = false;
        traceErr() << "Impossibile avere lo stato del device";
    }

    traceExit;
    return res;

}

bool GalilPLCInspector::isDeviceConnected() {

    traceEnter;
    bool isConnected = getGalilPLCDevicePtr()->isConnected();
    traceExit;
    return isConnected;

}

bool GalilPLCInspector::connectDevice() {
    traceEnter;
    bool res = getGalilPLCDevicePtr()->connect(ipAddress);
    traceExit;
    return res;
}

void GalilPLCInspector::handleDisconnection() {
    // do nothing
}
