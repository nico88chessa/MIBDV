#include "GalilPLCController.hpp"

using namespace PROGRAM_NAMESPACE;

GalilPLCController::GalilPLCController() :
    handler(nullptr), isInitialized(false), connected(false),
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

int GalilPLCController::getRecord(GalilPLCStatusBean& record) const {

    traceEnter;

    GDataRecord recordUnion;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GRecord(handle(), &recordUnion, G_QR);
#else
    GReturn result = G_NO_ERROR;
#endif
    record = GalilPLCStatusBean(recordUnion.rio47000);

    writeErrorIfExists(result);

    traceExit;

    return result;

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
     * NOTE NIC 18/10/2018 - metodo connect
     * se non riesco a connettermi al Galil, allora devo resettare il puntatore;
     * altrimenti nel distruttore viene lanciata un'eccezione
     **/


    if (result != G_NO_ERROR) {
        handler.reset();
        writeError(result);
    }

    this->connected = true;
    traceInfo() << "Galil PLC: connessione avvenuta";
    traceExit;

    return result == G_NO_ERROR;

}

int GalilPLCController::getDigitalInput(int input, int& inputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (input<1 || input>numDigitalInput) {
        traceErr() << "Galil PLC: input richiesto fuori dal range del device";
        return G_CUSTOM_PLC_DIGITAL_INPUT_OUT_OF_RANGE;
    }

    int bankStatus;
    int result = getInputs((input-1)/NUM_IO_PER_BANK, bankStatus);

    traceDebug() << "La funzione getInputs ha tornato il valore:" << bankStatus;
    inputStatus = (bankStatus & (0x01 << (input - 1))) >> (input - 1);

    traceDebug() << "Input" << input << ":" << inputStatus;

    traceExit;

    return result;

}

int GalilPLCController::getDigitalOutput(int output, int& outputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (output<1 || output>numDigitalOutput) {
        traceErr() << "Galil PLC: output richiesto fuori dal range del device";
        return G_CUSTOM_PLC_DIGITAL_OUTPUT_OUT_OF_RANGE;
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

    return result;

}

int GalilPLCController::getAnalogInput(int analogInput, anlType& analogInputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (analogInput<1 || analogInput>numAnalogInput) {
        traceErr() << "Galil PLC: output richiesto fuori dal range del device";
        return G_CUSTOM_PLC_ANALOG_INPUT_OUT_OF_RANGE;
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

    return result;

}

int GalilPLCController::setDigitalOutput(int output, bool value) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (output<1 || output>numDigitalOutput) {
        traceErr() << "Galil PLC: input richiesto fuori dal range del device";
        return G_CUSTOM_PLC_DIGITAL_OUTPUT_OUT_OF_RANGE;
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

    return result;

}

bool GalilPLCController::isConnected() const {

    traceEnter;
    traceExit;
    return this->connected;

}

int GalilPLCController::getTCCode(int& tcCode) const {

    QString command = QString("TC 0");
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &tcCode);
#else
    GReturn result = G_NO_ERROR;
#endif
    writeErrorIfExists(result);

    return result;

}

GalilPLCStatusBean GalilPLCController::getStatus() const {

    GalilPLCStatusBean record;
    writeErrorIfExists(this->getRecord(record));
    return record;

}

void GalilPLCController::writeError(int errorCode) const {

    traceErr() << "Galil PLC: codice errore:" << errorCode;
    traceErr() << "Galil PLC: descrizione errore:" << GalilControllerUtils::getErrorDescription(errorCode);

    if (errorCode == G_BAD_RESPONSE_QUESTION_MARK) {
        int tcCode;
        this->getTCCode(tcCode);
        traceErr() << "Galil PLC: dettagli errore:" << GalilControllerUtils::getTCDescription(tcCode);
    };

}

void GalilPLCController::writeErrorIfExists(int errorCode) const {

    if (errorCode != G_NO_ERROR)
        writeError(errorCode);

}

int GalilPLCController::getInputs(int bank, int& bankStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (bank<0 || bank>6) {
        traceErr() << "Galil PLC: la funzione get inputs accetta un valore compreso fra 0 e 6";
        return G_CUSTOM_PLC_DIGITAL_INPUT_OUT_OF_RANGE;
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

    return result;

}
