#include "GalilPLCInspector.hpp"

#include <Settings.hpp>

using namespace PROGRAM_NAMESPACE;

GalilPLCInspector::GalilPLCInspector(QObject* parent) :
    ConnectedDeviceInspector(parent) {

    Settings& s = Settings::instance();
    this->setDevice(new GalilPLCController());

    getGalilPLCDevicePtr()->setupController(s.getGalilPLCIpAddress(),
                                            s.getGalilPLCConnectionTimeoutMs(),
                                            s.getGalilPLCNumberDigitalInput(),
                                            s.getGalilPLCNumberDigitalOutput(),
                                            s.getGalilPLCNumberAnalogInput());

    this->setRefreshValue(s.getGalilPLCStatusRefreshIntervalMs());
    this->setRestartTime(s.getGalilPLCReconnectionIntervalMs());

}

//bool GalilPLCInspector::getStatus(QVariant& status) {

//    traceEnter;
//    bool res;

//    try {
//        GalilPLCStatusBean s = this->getGalilPLCDevicePtr()->getStatus();
//        status = QVariant::fromValue(s);
//        res = true;
//    } catch(NoStatusException& e) {
//        Q_UNUSED(e)
//        res = false;
//        traceErr() << "Impossibile avere lo stato del device";
//    }

//    traceExit;
//    return res;

//}

bool GalilPLCInspector::isDeviceConnected() {

    traceEnter;
    bool isConnected = getGalilPLCDevicePtr()->isConnected();
    traceExit;
    return isConnected;

}

bool GalilPLCInspector::connectDevice() {
    traceEnter;
    bool res = getGalilPLCDevicePtr()->connect();
    traceExit;
    return res;
}

void GalilPLCInspector::handleDisconnection() {
    // do nothing
}
