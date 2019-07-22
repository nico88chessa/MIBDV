#include "GalilPLCInspector.hpp"

#include <Settings.hpp>


using namespace PROGRAM_NAMESPACE;

static const ErrorID GALIL_PLC_INSPECTOR_FAILED_TO_CONNECT = PROGRAM_ERR_START_CODE + 1;
static const ErrorID GALIL_PLC_INSPECTOR_DISCONNECTED = PROGRAM_ERR_START_CODE + 2;

static constexpr char GALIL_PLC_INSPECTOR_FAILED_TO_CONNECT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Impossible to connect to device in Galil PLC Inspector");
static constexpr char GALIL_PLC_INSPECTOR_DISCONNECTED_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Galil PLC Inspector device disconnected");

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

    initializeErrorSignaler(errorSignaler, this);

    connect(this, &AbstractConnectedDeviceInspector::failedToConnectSignal, [&]() {
        errorSignaler->clearErrors();
        errorSignaler->addError(Error(DeviceKey::GALIL_PLC_INSPECTOR, GALIL_PLC_INSPECTOR_FAILED_TO_CONNECT, GALIL_PLC_INSPECTOR_FAILED_TO_CONNECT_DESCR, ErrorType::FATAL));
        errorSignaler->notifyErrors();
    });

    connect(this, &AbstractConnectedDeviceInspector::disconnectedSignal, [&]() {
        errorSignaler->clearErrors();
        errorSignaler->addError(Error(DeviceKey::GALIL_PLC_INSPECTOR, GALIL_PLC_INSPECTOR_DISCONNECTED, GALIL_PLC_INSPECTOR_DISCONNECTED_DESCR, ErrorType::FATAL));
        errorSignaler->notifyErrors();
    });

    connect(this, &AbstractConnectedDeviceInspector::connectedSignal, [&]() {
        errorSignaler->clearErrors();
        errorSignaler->notifyErrors();
    });

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
