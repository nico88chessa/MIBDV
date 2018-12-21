#include "GalilPLCController.hpp"

#include <gclib.h>
#include <gclibo.h>
#include <Logger.hpp>
#include <devices/galil/GalilControllerUtils.hpp>

using namespace PROGRAM_NAMESPACE;

GalilPLCController::GalilPLCController() :
    handler(new GCon), isInitialized(false), connected(false),
    numDigitalInput(0), numDigitalOutput(0), numAnalogInput(0), handleCode("") {

    traceEnter;

    /* NOTE NIC 07/11/2018 - handler inizializzato alla creazione dell'istanza
     * IMPORTANTE: istanzio un puntatore non valido, verra' validato alla creazione
     * della connessione; se la creazione della connessione al Galil non va a buon
     * fine, il puntatore e' ancora non valido (se lo uso in una chiamata ad una
     * funzione del galil, viene generato un segmentation fault)
     */

    traceExit;

}

GalilPLCController::~GalilPLCController() {

    traceEnter;
    this->disconnect();
    traceExit;

}

int GalilPLCController::getRecord(GalilPLCStatusBean& record) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

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

    if (isConnected())
        return true;

    // TODO NIC 07/11/2018 - parametrizzare da file di configurazione
    int timeout = 1000;
    QString command = ip + QString(" -t %1").arg(timeout);

#ifdef FLAG_PLC_PRESENT
    GReturn result = GOpen(command.toStdString().data(), handler.data());
#else
    GReturn result = G_NO_ERROR;
#endif

    /**
     * NOTE NIC 07/11/2018 - metodo connect
     * se non riesco a connettermi al Galil, allora il puntatore non viene inizializzato;
     * quindi non e' utilizzabile
     **/


    if (result == G_NO_ERROR) {
        this->setConnected(true);
        traceInfo() << "Galil PLC: connessione avvenuta";

        command = QString("WH");
        char out[1024] = {};
        char* outPtr = nullptr;

        traceInfo() << "Galil PLC: prendo l'handleCode della connessione attuale";
        traceDebug() << "Invio comando:" << command;
    #ifdef FLAG_PLC_PRESENT
        result = GCmdT(handle(), command.toStdString().data(), out, sizeof(out), &outPtr);
    #else
        result = G_NO_ERROR;
    #endif
        writeErrorIfExists(result);

        if (result == G_NO_ERROR) {

            QString currentHandleCode(outPtr);
            if (!handleCode.isEmpty()) {

                if (handleCode.compare(currentHandleCode) != 0) {
                    traceInfo() << "Galil PLC: forzo chiusura di una precedente connessione con handle:" << handleCode;

                    command = handleCode + "=>-3";
                    traceDebug() << "Invio comando:" << command;
                    result = GCmd(handle(), command.toStdString().data());
                    writeErrorIfExists(result);

                }

            }
            handleCode = currentHandleCode;
        }

    }

    traceExit;

    return result == G_NO_ERROR;

}

int GalilPLCController::getDigitalInput(int input, int& inputStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (input<0 || input>numDigitalInput) {
        traceErr() << "Galil PLC: input richiesto fuori dal range del device";
        return G_CUSTOM_PLC_DIGITAL_INPUT_OUT_OF_RANGE;
    }

    int bankStatus;
    int result = getInputs(input/NUM_IO_PER_BANK, bankStatus);

    traceDebug() << "La funzione getInputs ha tornato il valore:" << bankStatus;
    inputStatus = (bankStatus & (0x01 << input)) >> input;

    traceDebug() << "Input" << input << ":" << inputStatus;

    traceExit;

    return result;

}

int GalilPLCController::getDigitalOutput(int output, int& outputStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (output<0 || output>numDigitalOutput) {
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

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

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

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil PLC: il controller non e' stato inizializzato";
        return G_CUSTOM_PLC_NON_INIZIALIZZATO;
    }

    if (output<0 || output>numDigitalOutput) {
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

int GalilPLCController::getTCCode(int& tcCode) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

    QString command = QString("TC 0");
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_PLC_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &tcCode);
#else
    GReturn result = G_NO_ERROR;
#endif
    writeErrorIfExists(result);

    traceExit;

    return result;

}

GalilPLCStatusBean GalilPLCController::getStatus() {

    GalilPLCStatusBean record;
    GReturn result = this->getRecord(record);
    writeErrorIfExists(result);

    if (this->isError(result))
        throw NoStatusException();

    return record;

}

QString GalilPLCController::decodeError(const int& errorCode) {

    return GalilControllerUtils::getErrorDescription(errorCode);

}

void GalilPLCController::writeError(int errorCode) {

    traceErr() << "Galil PLC: codice errore:" << errorCode;
    traceErr() << "Galil PLC: descrizione errore:" << GalilControllerUtils::getErrorDescription(errorCode);

    if (errorCode == G_BAD_RESPONSE_QUESTION_MARK) {

        int tcCode;
        this->getTCCode(tcCode);
        traceErr() << "Galil PLC: dettagli errore:" << GalilControllerUtils::getTCDescription(tcCode);

    } else if (errorCode == G_TIMEOUT || errorCode == G_READ_ERROR || errorCode == G_WRITE_ERROR)
        /* NOTE NIC 07/11/2018 - mi accorgo se il PLC e' connesso dopo l'invio di comando;
         * il dispositivo non e' connesso se il comando da errore di timeout (1100)
         */
        this->disconnect();

}

void GalilPLCController::writeErrorIfExists(int errorCode) {

    if (errorCode != G_NO_ERROR)
        writeError(errorCode);

}

int GalilPLCController::getInputs(int bank, int& bankStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

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

int GalilPLCController::disconnect() {

    traceEnter;

    if (!this->isConnected()) {
        traceInfo() << "Galil PLC: connessione non presente; nessuna sconnessione da effettuare";
        return G_NO_ERROR;
    }

#ifdef FLAG_PLC_PRESENT
    GReturn result = GClose(handle());
    if (result == G_NO_ERROR)
        this->setConnected(false);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;
    return result;

}

int GalilPLCController::getKeepAliveTimeMs(unsigned int* timeMs, unsigned int* newValue) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil PLC: il controller non e' connesso";
        return G_CUSTOM_PLC_NOT_CONNECTED;
    }

#ifdef FLAG_PLC_PRESENT
    GReturn result = GUtility(handle(), G_UTIL_GCAPS_KEEPALIVE, timeMs, newValue);
#else
    *timeMs = 600*1000; // valore default
    GReturn result = G_NO_ERROR;
#endif

    traceExit;
    return result;

}
