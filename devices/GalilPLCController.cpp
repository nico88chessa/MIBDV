#include "GalilPLCController.hpp"

using namespace PROGRAM_NAMESPACE;

GalilPLCController::GalilPLCController() :
    handler(nullptr), isInitialized(false),
    numDigitalInput(0), numDigitalOutput(0), numAnalogInput(0) {

    traceEnter;
    traceExit;

}

GalilPLCController::~GalilPLCController() {

    traceEnter;

    GReturn result = G_NO_ERROR;

    if (!this->handler.isNull())
        result = GClose(handle());

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

}

bool GalilPLCController::getRecord(GDataRecord47000_ENC& record) {

    traceEnter;

    GDataRecord recordUnion;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GRecord(handle(), &recordUnion, G_QR);
#else
    GReturn result = G_NO_ERROR;
#endif
    record = recordUnion.rio47000;

    writeErrorIfExists(result);

    traceExit;

    return result == G_NO_ERROR;

}

void GalilPLCController::setupController(int numDigitalInput, int numDigitalOutput, int numAnalogInput) {

    traceEnter;

    if (isInitialized)
        return;

    this->numDigitalInput = numDigitalInput;
    this->numDigitalOutput = numDigitalOutput;
    this->numAnalogInput = numAnalogInput;

    isInitialized = true;

    traceExit;

}

bool GalilPLCController::connect(const QString& ip) {

    traceEnter;

    if (!this->handler.isNull())
        return true;
    else
        handler.reset(new GCon);

    GReturn result = GOpen(ip.toStdString().data(), handler.data());
//    QString str = ip + " -d";
//    GReturn result = GOpen(str.toStdString().data(), handle.data());

    /**
     * NOTE NIC 2018/10/18 - metodo connect
     * se non riesco a connettermi al Galil, allora devo resettare il puntatore;
     * altrimenti nel distruttore viene lanciata un'eccezione
     **/

    if (result != G_NO_ERROR) {
        handler.reset();
        writeError(result);
    }

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilPLCController::getDigitalInput(int input, int& inputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return false;
    }

    if (input<1 || input>numDigitalInput) {
        traceErr() << "Galil PLC: input richiesto fuori dal range del device";
        return false;
    }

    int bankStatus;
    if (!getInputs((input-1)/NUM_IO_PER_BANK, bankStatus))
        return false;

    traceDebug() << "La funzione getInputs ha tornato il valore:" << bankStatus;
    inputStatus = (bankStatus & (0x01 << (input - 1))) >> (input - 1);

    traceDebug() << "Input" << input << ":" << inputStatus;

    traceExit;

    return true;

}

bool GalilPLCController::getDigitalOutput(int output, int& outputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return false;
    }

    if (output<1 || output>numDigitalOutput) {
        traceErr() << "Galil PLC: output richiesto fuori dal range del device";
        return false;
    }

    QString command = QString("MG @OUT[%1]").arg(output);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &outputStatus);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilPLCController::getAnalogInput(int analogInput, anlType& analogInputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return false;
    }

    if (analogInput<1 || analogInput>numAnalogInput) {
        traceErr() << "Galil PLC: output richiesto fuori dal range del device";
        return false;
    }

    QString command = QString("MG @AN[%1]").arg(analogInput);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GCmdD(handle(), command.toStdString().data(), &analogInputStatus);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilPLCController::setDigitalOutput(int output, bool value) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return false;
    }

    if (output<1 || output>numDigitalOutput) {
        traceErr() << "Galil PLC: input richiesto fuori dal range del device";
        return false;
    }

    QString command = value ? "SB " : "CB ";
    command.append(QString::number(output));

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result == G_NO_ERROR;

}

void GalilPLCController::writeError(int errorCode) {

    traceErr() << "Galil PLC: codice errore:" << errorCode;
    traceErr() << "Galil PLC: descrizione errore:" << GalilCNControllerUtils::getErrorString(errorCode);

}

void GalilPLCController::writeErrorIfExists(int errorCode) {

    if (errorCode != G_NO_ERROR)
        writeError(errorCode);

}

bool GalilPLCController::getInputs(int bank, int& bankStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return false;
    }

    if (bank<0 || bank>6) {
        traceErr() << "Galil PLC: la funzione get inputs accetta un valore compreso fra 0 e 6";
        return false;
    }

    QString command = "TI " + QString::number(bank);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().c_str(), &bankStatus);
#else
    GReturn result = G_NO_ERROR;
#endif
    writeErrorIfExists(result);


    traceExit;

    return result == G_NO_ERROR;

}
