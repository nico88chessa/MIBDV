#include "GalilCNInspector.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNInspector::GalilCNInspector(QObject* parent) : QObject(parent),
    errorSignaler(new ErrorSignaler()) {

    // TODO NIC 26/10/2018 - correggere con parametri in file di configurazione
    refreshTimer.setInterval(1000);

    connect(&refreshTimer, &QTimer::timeout, this, &GalilCNInspector::process);


}

GalilCNInspector::~GalilCNInspector() {

    // errorSignaler viene eliminato automaticamente
    refreshTimer.stop();
}

void GalilCNInspector::process() {

    GDataRecord2103 status;
//    GDataRecord2103 status = controller.getStatus();
//    GalilCNStatusBean bean(status);
    auto statusPlc = controllerPlc.getStatus();
    GalilPLCStatusBean bean2(statusPlc);

//    traceDebug() << "input:" << static_cast<int>(bean.getDigitalInput1()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput2()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput3()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput4()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput5()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput6()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput7()) << "-" <<
//                    static_cast<int>(bean.getDigitalInput8());
//    traceDebug() << "output:" << static_cast<int>(bean.getDigitalOutput1()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput2()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput3()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput4()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput5()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput6()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput7()) << "-" <<
//                    static_cast<int>(bean.getDigitalOutput8());
//    traceDebug() << "switchesA:" <<
//                    static_cast<int>(bean.getAxisALatchOccured()) << "-" <<
//                    static_cast<int>(bean.getAxisAStateLatchInput()) << "-" <<
//                    static_cast<int>(bean.getAxisAForwardLimit()) << "-" <<
//                    static_cast<int>(bean.getAxisAReverseLimit()) << "-" <<
//                    static_cast<int>(bean.getAxisAHomeInput()) << "-" <<
//                    static_cast<int>(bean.getAxisASMJumperInstalled());
//    traceDebug() << "switchesB:" <<
//                    static_cast<int>(bean.getAxisBLatchOccured()) << "-" <<
//                    static_cast<int>(bean.getAxisBStateLatchInput()) << "-" <<
//                    static_cast<int>(bean.getAxisBForwardLimit()) << "-" <<
//                    static_cast<int>(bean.getAxisBReverseLimit()) << "-" <<
//                    static_cast<int>(bean.getAxisBHomeInput()) << "-" <<
//                    static_cast<int>(bean.getAxisBSMJumperInstalled());
//    traceDebug() << "switchesC:" <<
//                    static_cast<int>(bean.getAxisCLatchOccured()) << "-" <<
//                    static_cast<int>(bean.getAxisCStateLatchInput()) << "-" <<
//                    static_cast<int>(bean.getAxisCForwardLimit()) << "-" <<
//                    static_cast<int>(bean.getAxisCReverseLimit()) << "-" <<
//                    static_cast<int>(bean.getAxisCHomeInput()) << "-" <<
//                    static_cast<int>(bean.getAxisCSMJumperInstalled());

    traceDebug() << "input:" << static_cast<int>(bean2.getDigitalInput1()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput2()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput3()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput4()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput5()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput6()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput7()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput8()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput9()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput10()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput11()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput12()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput13()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput14()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput15()) << "-" <<
                    static_cast<int>(bean2.getDigitalInput16());

    traceDebug() << "output:" << static_cast<int>(bean2.getDigitalOutput1()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput2()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput3()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput4()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput5()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput6()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput7()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput8()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput9()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput10()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput11()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput12()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput13()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput14()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput15()) << "-" <<
                    static_cast<int>(bean2.getDigitalOutput16());








//    errorSignaler->addError(Error(
//                                static_cast<int>(GalilCNController::deviceKey),
//                                1,
//                                "ciao",
//                                ErrorType::ERROR));

    emit statusSignal(status);

}

void GalilCNInspector::restartProcess() {

}



void GalilCNInspector::startProcess() {

    traceEnter;

    // TODO NIC 26/10/2018 - correggere con parametri in file di configurazione
//    controller.setupController(8, 8, 0);
//    QString ip = "169.254.12.10";
    int reconnectTimeMs = 1000;
//    if (!controller.connect(ip)) {
//        traceDebug() << "GalilCNInspector: connessione al CN fallita";
//        QTimer::singleShot(reconnectTimeMs, this, &GalilCNInspector::startProcess);
//    }

    controllerPlc.setupController(16, 16, 8);
    QString ipPlc = "169.254.12.11";
    if (!controllerPlc.connect(ipPlc)) {
        traceDebug() << "GalilCNInspector: connessione al CN fallita";
        QTimer::singleShot(reconnectTimeMs, this, &GalilCNInspector::startProcess);
    }

    refreshTimer.start();

    emit processStartSignal();
    traceExit;

}

void GalilCNInspector::stopProcess() {

    emit processStopSignal();
}
