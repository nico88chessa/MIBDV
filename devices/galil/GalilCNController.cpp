#include "GalilCNController.hpp"

#include <gclib.h>
#include <gclibo.h>
#include <Logger.hpp>
#include <devices/galil/GalilControllerUtils.hpp>

using namespace PROGRAM_NAMESPACE;

GalilCNController::GalilCNController() :
    handler(new GCon), isInitialized(false), connected(false),
    numDigitalInput(0), numDigitalOutput(0), numAnalogInput(0) {

    traceEnter;

    /* NOTE NIC 07/11/2018 - handler inizializzato alla creazione dell'istanza
     * IMPORTANTE: istanzio un puntatore non valido, verra' validato alla creazione
     * della connessione; se la creazione della connessione al Galil non va a buon
     * fine, il puntatore e' ancora non valido (se lo uso in una chiamata ad una
     * funzione del galil, viene generato un segmentation fault)
     */

    traceExit;

}

GalilCNController::~GalilCNController() {

    traceEnter;
    this->disconnect();
    traceExit;

}

int GalilCNController::getRecord(GalilCNStatusBean& record) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    GDataRecord recordUnion;
#ifdef FLAG_CN_PRESENT
    GReturn result = GRecord(handle(), &recordUnion, G_QR);
#else
    GReturn result = G_NO_ERROR;
#endif
    record = GalilCNStatusBean(recordUnion.dmc2103);

    writeErrorIfExists(result);

    traceExit;

    return result;

}

void GalilCNController::setupController(int numDigitalInput, int numDigitalOutput, int numAnalogInput) {

    traceEnter;

    if (isInitialized)
        return;

    this->numDigitalInput = numDigitalInput;
    this->numDigitalOutput = numDigitalOutput;
    this->numAnalogInput = numAnalogInput;

    isInitialized = true;

    traceExit;

}

bool GalilCNController::connect(const QString& ip) {

    traceEnter;

    if (isConnected())
        return true;

    // TODO NIC 07/11/2018 - parametrizzare da file di configurazione
    int timeout = 1000;
    QString command = ip + QString(" -t %1").arg(timeout);

#ifdef FLAG_CN_PRESENT
    GReturn result = GOpen(command.toStdString().data(), handler.data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);
    /**
     * NOTE NIC 07/11/2018 - metodo connect
     * se non riesco a connettermi al Galil, allora il puntatore non viene inizializzato;
     * quindi non e' utilizzabile
     **/
    if (result == G_NO_ERROR) {
        this->setConnected(true);
        traceInfo() << "Galil CN: connessione avvenuta";
    }

    traceExit;

    return result == G_NO_ERROR;

}

int GalilCNController::getDigitalInput(int input, int& inputStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (input<0 || input>numDigitalInput) {
        traceErr() << "Galil CN: input richiesto fuori dal range del device";
        return G_CUSTOM_CN_DIGITAL_INPUT_OUT_OF_RANGE;
    }

    int bankStatus;
    int result = getInputs(input/NUM_IO_PER_BANK, bankStatus);

    traceDebug() << "La funzione getInputs ha tornato il valore:" << bankStatus;
    inputStatus = (bankStatus & (0x01 << input)) >> input;

    traceDebug() << "Input" << input << ":" << inputStatus;

    traceExit;

    return result;

}

int GalilCNController::getDigitalOutput(int output, int& outputStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (output<0 || output>numDigitalOutput) {
        traceErr() << "Galil CN: output richiesto fuori dal range del device";
        return G_CUSTOM_CN_DIGITAL_OUTPUT_OUT_OF_RANGE;
    }

    QString command = QString("MG @OUT[%1]").arg(output);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &outputStatus);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::getAnalogInput(int analogInput, anlType& analogInputStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (analogInput<0 || analogInput>numAnalogInput) {
        traceErr() << "Galil CN: output richiesto fuori dal range del device";
        return G_CUSTOM_CN_ANALOG_INPUT_OUT_OF_RANGE;
    }

    QString command = QString("MG @AN[%1]").arg(analogInput);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdD(handle(), command.toStdString().data(), &analogInputStatus);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::getPosition(Axis a, posCNType& pos) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = "MG _MT" + axisLetter;
    int motorType;

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &motorType);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    command.clear();

    if (qAbs(motorType)==1)
        command = "TP" + axisLetter; // servo motor
    else
        command = "RP" + axisLetter; // step motor

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    result = GCmdI(handle(), command.toStdString().data(), &pos);
#else
    result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceDebug() << "Posizione asse" << axisLetter << ":" << pos;

    traceExit;

    return result;

}

int GalilCNController::isAxisInMotion(Axis a, bool& inMotion) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    int value = 0;
    GReturn result = tellSwitches(a, value);
    inMotion = value & (0x01 << static_cast<int>(SwitchBitMask::AXIS_IN_MOTION));

    traceExit;

    return result;

}

int GalilCNController::isAxisPositionError(Axis a, bool& isPositionError) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isPositionError = value & (0x01 << static_cast<int>(SwitchBitMask::POSITION_ERROR_EXCEEDS_ERROR_LIMIT));

    traceExit;

    return result;

}

int GalilCNController::isMotorOff(Axis a, bool& isMotorOff) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isMotorOff = value & (0x01 << static_cast<int>(SwitchBitMask::MOTOR_OFF));

    traceExit;

    return result;

}

int GalilCNController::isForwardLimit(Axis a, bool& isForwardLimit) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isForwardLimit = value & (0x01 << static_cast<int>(SwitchBitMask::FORWARD_LIMIT_SWITCH_INACTIVE));

    traceExit;

    return result;

}

int GalilCNController::isBackwardLimit(Axis a, bool& isBackwardLimit) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isBackwardLimit = value & (0x01 << static_cast<int>(SwitchBitMask::REVERSE_LIMIT_SWITCH_INACTIVE));

    traceExit;

    return result;

}

int GalilCNController::isHomeAxis(Axis a, bool& isHome) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isHome = value & (0x01 << static_cast<int>(SwitchBitMask::HOME_SWITCH_STATUS));

    traceExit;

    return result;

}

bool GalilCNController::checkAbort(bool& isAbort) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString command = "MG _AB";
    int value;
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &value);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);
    isAbort = value == 1;

    traceExit;
    return result;

}

int GalilCNController::setDigitalOutput(int output, bool value) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (output<0 || output>numDigitalOutput) {
        traceErr() << "Galil CN: output richiesto fuori dal range del device";
        return G_CUSTOM_CN_DIGITAL_OUTPUT_OUT_OF_RANGE;
    }

    QString command = value ? "SB " : "CB ";
    command.append(QString::number(output));

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::setSpeeds(Axis a, spdCNType speed) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("SP%1 = %2").arg(axisLetter).arg(speed);

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    traceExit;

    return result;

}

int GalilCNController::setAccelerations(Axis a, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("AC%1 = %2").arg(axisLetter).arg(acc);

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    command = QString("DC%1 = %2").arg(axisLetter).arg(dec);
    traceDebug() << "Invio comando:" << command;

#ifdef FLAG_CN_PRESENT
    result = GCmd(handle(), command.toStdString().data());
#else
    result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::setMoveParameters(Axis a, spdCNType speed, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    GReturn result;
    result = this->setSpeeds(a, speed);
    if (result == G_NO_ERROR)
        result = this->setAccelerations(a, acc, dec);

    traceExit;

    return result;

}

int GalilCNController::stopAxis(Axis a) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("ST%1").arg(axisLetter);

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::homingX(spdCNType speed, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    Axis a = Axis::X;
    GReturn result;
    result = this->setMoveParameters(a, speed, acc, dec);

    if (result != G_NO_ERROR)
        return result;

    QString command = QString("FE%1").arg(GalilCNController::letterFromAxis(a));
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    result = GCmd(handle(), command.toStdString().data());
#else
    result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::homingY(spdCNType speed, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    Axis a = Axis::Y;
    GReturn result;
    result = this->setMoveParameters(a, speed, acc, dec);

    if (result != G_NO_ERROR)
        return result;

    QString command = QString("HM%1").arg(GalilCNController::letterFromAxis(a));
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    result = GCmd(handle(), command.toStdString().data());
#else
    result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::homingZ(spdCNType speed, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    Axis a = Axis::Z;
    GReturn result;
    result = this->setMoveParameters(a, speed, acc, dec);

    if (result != G_NO_ERROR)
        return result;

    QString command = QString("HM%1").arg(GalilCNController::letterFromAxis(a));
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    result = GCmd(handle(), command.toStdString().data());
#else
    result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::homingW(MAYBE_UNUSED spdCNType speed, MAYBE_UNUSED accCNType acc, MAYBE_UNUSED accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    // per ora il quarto asse non fa nulla
    traceExit;

    return true;

}

int GalilCNController::home(Axis a, spdCNType speed, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    bool result;
    switch (a) {
        case Axis::X: result = homingX(speed, acc, dec); break;
        case Axis::Y: result = homingY(speed, acc, dec); break;
        case Axis::Z: result = homingZ(speed, acc, dec); break;
        case Axis::W: result = homingW(speed, acc, dec); break;
    }
    traceExit;

    return result;

}

int GalilCNController::startMoveAxis(Axis a) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("BG%1").arg(axisLetter);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::moveToPosition(Axis a, posCNType pos, spdCNType speed, accCNType acc, accCNType dec) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    GReturn result;

    result = this->setMoveParameters(a, speed, acc, dec);

    if (result != G_NO_ERROR)
        return result;

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("PA%1 = %2").arg(axisLetter).arg(pos);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    result = GCmd(handle(), command.toStdString().data());
#else
    result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::setPosition(Axis a, posCNType pos) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("DP%1 = %2").arg(axisLetter).arg(pos);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmd(handle(), command.toStdString().data());
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

bool GalilCNController::isConnected() const {

    traceEnter;
    traceExit;
    return this->connected;

}

int GalilCNController::getTCCode(int& tcCode) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString command = QString("TC 0");
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &tcCode);
#else
    GReturn result = G_NO_ERROR;
#endif
    writeErrorIfExists(result);

    traceExit;

    return result;

}

GalilCNStatusBean GalilCNController::getStatus() {

    GalilCNStatusBean record;
    GReturn result = this->getRecord(record);
    writeErrorIfExists(result);

    if (this->isError(result))
        throw NoStatusException();

    return record;

}

QString GalilCNController::decodeError(const int& errorCode) {

    return GalilControllerUtils::getErrorDescription(errorCode);

}

int GalilCNController::getKeepAliveTimeMs(unsigned int* timeMs, unsigned int* newValue) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

#ifdef FLAG_CN_PRESENT
    GReturn result = GUtility(handle(), G_UTIL_GCAPS_KEEPALIVE, timeMs, newValue);
#else
    *timeMs = 600*1000; // valore default
    GReturn result = G_NO_ERROR;
#endif

    traceExit;
    return result;

}

void GalilCNController::writeError(int errorCode) {

    traceErr() << "Galil CN: codice errore:" << errorCode;
    traceErr() << "Galil CN: descrizione errore:" << GalilControllerUtils::getErrorDescription(errorCode);

    if (errorCode == G_BAD_RESPONSE_QUESTION_MARK) {

        int tcCode;
        this->getTCCode(tcCode);
        traceErr() << "Galil CN: dettagli errore:" << GalilControllerUtils::getTCDescription(tcCode);

    } else if (errorCode == G_TIMEOUT || errorCode == G_READ_ERROR || errorCode == G_WRITE_ERROR)
        /* NOTE NIC 07/11/2018 - mi accorgo se il CN e' connesso dopo l'invio di comando;
         * il dispositivo non e' connesso se il comando da errore di timeout (1100)
         */
        this->disconnect();


}

void GalilCNController::writeErrorIfExists(int errorCode) {

    if (errorCode != G_NO_ERROR)
        writeError(errorCode);

}

int GalilCNController::getInputs(int bank, int& bankStatus) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (bank<0 || bank>6) {
        traceErr() << "Galil CN: la funzione get inputs accetta un valore compreso fra 0 e 6";
        return G_CUSTOM_CN_DIGITAL_INPUT_OUT_OF_RANGE;
    }

    QString command = "TI " + QString::number(bank);
    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().c_str(), &bankStatus);
#else
    GReturn result = G_NO_ERROR;
#endif
    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::tellSwitches(Axis a, int& value) {

    traceEnter;

    if (!isConnected()) {
        traceErr() << "Galil CN: il controller non e' connesso";
        return G_CUSTOM_CN_NOT_CONNECTED;
    }

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = "TS" + axisLetter;

    traceDebug() << "Invio comando:" << command;
#ifdef FLAG_CN_PRESENT
    GReturn result = GCmdI(handle(), command.toStdString().data(), &value);
#else
    GReturn result = G_NO_ERROR;
#endif

    writeErrorIfExists(result);

    traceExit;

    return result;

}

int GalilCNController::disconnect() {

    traceEnter;

    if (!this->isConnected()) {
        traceInfo() << "Galil CN: connessione non presente; nessuna disconnessione da effettuare";
        return G_NO_ERROR;
    }

#ifdef FLAG_CN_PRESENT
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
