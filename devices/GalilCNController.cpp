#include "GalilCNController.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNController::GalilCNController() : handle(nullptr) {

    traceEnter;

    traceExit;

}

GalilCNController::~GalilCNController() {

    traceEnter;

    GReturn value = G_NO_ERROR;

    if (!this->handle.isNull())
        value = GClose(*handle.data());

    if (G_NO_ERROR != value)
        traceErr() << "impossibile connettersi al device; codice errore:" << value;

    traceExit;

}

bool GalilCNController::connect(const QString& ip) {

    traceEnter;

    if (!this->handle.isNull())
        return true;
    else
        handle.reset(new GCon);

    GReturn value;
    QString str = ip + " -d";
//    value = GOpen(ip.toStdString().data(), handle.data());
    value = GOpen(str.toStdString().data(), handle.data());

    traceDebug() << GalilCNControllerUtils::getErrorString(value);

    /**
     * NOTE NIC 2018/10/18
     * se non riesco a connettermi al Galil, allora devo resettare il puntatore;
     * altrimenti nel distruttore viene lanciata un'eccezione
     **/

    if (G_NO_ERROR != value) {
        handle.reset();
        traceErr() << "impossibile connettersi al device: codice errore:" << value;
    }

    traceExit;

    return true;

}
