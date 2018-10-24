#include "GalilCNController.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNController::GalilCNController() :
    handler(nullptr), isInitialized(false), connected(false),
    numDigitalInput(0), numDigitalOutput(0), numAnalogInput(0) {

    traceEnter;
    traceExit;

}

GalilCNController::~GalilCNController() {

    traceEnter;

    GReturn result = G_NO_ERROR;

    if (!this->handler.isNull())
        result = GClose(handle());

    writeErrorIfExists(result);

    traceExit;

}

int GalilCNController::getRecord(GDataRecord2103& record) const {

    traceEnter;

    GDataRecord recordUnion;
#ifdef FLAG_CN_PRESENT
    GReturn result = GRecord(handle(), &recordUnion, G_QR);
#else
    GReturn result = G_NO_ERROR;
#endif
    record = recordUnion.dmc2103;

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

    this->connected = true;
    traceInfo() << "Galil CN: connessione avvenuta";
    traceExit;

    return result == G_NO_ERROR;

}

int GalilCNController::getDigitalInput(int input, int& inputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (input<1 || input>numDigitalInput) {
        traceErr() << "Galil CN: input richiesto fuori dal range del device";
        return G_CUSTOM_CN_DIGITAL_INPUT_OUT_OF_RANGE;
    }

    int bankStatus;
    int result = getInputs((input-1)/NUM_IO_PER_BANK, bankStatus);

    traceDebug() << "La funzione getInputs ha tornato il valore:" << bankStatus;
    inputStatus = (bankStatus & (0x01 << (input - 1))) >> (input - 1);

    traceDebug() << "Input" << input << ":" << inputStatus;

    traceExit;

    return result;

}

int GalilCNController::getDigitalOutput(int output, int& outputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (output<1 || output>numDigitalOutput) {
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

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (analogInput<1 || analogInput>numAnalogInput) {
        traceErr() << "Galil CN: output richiesto fuori dal range del device";
        return G_CUSTOM_CN_ANALOGIC_INPUT_OUT_OF_RANGE;
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

int GalilCNController::getPosition(GalilCNController::Axis a, GalilCNController::posType& pos) {

    traceEnter;

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

int GalilCNController::isAxisInMotion(GalilCNController::Axis a, bool& inMotion) {

    traceEnter;

    int value = 0;
    GReturn result = tellSwitches(a, value);
    inMotion = value & (0x01 << static_cast<int>(SwitchBitMask::AXIS_IN_MOTION));

    traceExit;

    return result;

}

int GalilCNController::isAxisPositionError(GalilCNController::Axis a, bool& isPositionError) {

    traceEnter;

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isPositionError = value & (0x01 << static_cast<int>(SwitchBitMask::POSITION_ERROR_EXCEEDS_ERROR_LIMIT));

    traceExit;

    return result;

}

int GalilCNController::isMotorOff(GalilCNController::Axis a, bool& isMotorOff) {

    traceEnter;

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isMotorOff = value & (0x01 << static_cast<int>(SwitchBitMask::MOTOR_OFF));

    traceExit;

    return result;

}

int GalilCNController::isForwardLimit(GalilCNController::Axis a, bool& isForwardLimit) {

    traceEnter;

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isForwardLimit = value & (0x01 << static_cast<int>(SwitchBitMask::FORWARD_LIMIT_SWITCH_INACTIVE));

    traceExit;

    return result;

}

int GalilCNController::isBackwardLimit(GalilCNController::Axis a, bool& isForwardLimit) {

    traceEnter;

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isForwardLimit = value & (0x01 << static_cast<int>(SwitchBitMask::FORWARD_LIMIT_SWITCH_INACTIVE));

    traceExit;

    return result;

}

int GalilCNController::isHomeAxis(GalilCNController::Axis a, bool& isHome) {

    traceEnter;

    int value = 0;
    GReturn result = tellSwitches(a, value);
    isHome = value & (0x01 << static_cast<int>(SwitchBitMask::HOME_SWITCH_STATUS));

    traceExit;

    return result;

}

bool GalilCNController::checkAbort(bool& isAbort) {

    traceEnter;

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

    if (!isInitialized) {
        traceErr() << "Galil CN: il controller non e' stato inizializzato";
        return G_CUSTOM_CN_NON_INIZIALIZZATO;
    }

    if (output<1 || output>numDigitalOutput) {
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

int GalilCNController::setSpeeds(GalilCNController::Axis a, GalilCNController::spdType speed) {

    traceEnter;

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

int GalilCNController::setAccelerations(GalilCNController::Axis a, GalilCNController::accType acc, GalilCNController::accType dec) {

    traceEnter;

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

int GalilCNController::setMoveParameters(GalilCNController::Axis a, GalilCNController::spdType speed, GalilCNController::accType acc, GalilCNController::accType dec) {

    traceEnter;

    GReturn result;
    result = this->setSpeeds(a, speed);
    if (result == G_NO_ERROR)
        result = this->setAccelerations(a, acc, dec);

    traceExit;

    return result;

}

int GalilCNController::stopAxis(GalilCNController::Axis a) {

    traceEnter;

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

int GalilCNController::homingX(GalilCNController::spdType speed, GalilCNController::accType acc, GalilCNController::accType dec) {

    traceEnter;

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

int GalilCNController::homingY(GalilCNController::spdType speed, GalilCNController::accType acc, GalilCNController::accType dec) {

    traceEnter;

    Axis a = Axis::X;
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

int GalilCNController::homingZ(GalilCNController::spdType speed, GalilCNController::accType acc, GalilCNController::accType dec) {

    traceEnter;

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

int GalilCNController::homingW(MAYBE_UNUSED GalilCNController::spdType speed, MAYBE_UNUSED GalilCNController::accType acc, MAYBE_UNUSED GalilCNController::accType dec) {

    traceEnter;
    // per ora il quarto asse non fa nulla
    traceExit;

    return true;

}

int GalilCNController::home(GalilCNController::Axis a, GalilCNController::spdType speed, GalilCNController::accType acc, GalilCNController::accType dec) {

    traceEnter;

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

int GalilCNController::startMoveAxis(GalilCNController::Axis a) {

    traceEnter;

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

int GalilCNController::moveToPosition(Axis a, posType pos, spdType speed, accType acc, accType dec) {

    traceEnter;

    if (!this->setMoveParameters(a, speed, acc, dec))
        return false;

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = QString("PA%1 = %2").arg(axisLetter).arg(pos);
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

int GalilCNController::setPosition(GalilCNController::Axis a, GalilCNController::posType pos) {

    traceEnter;

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

int GalilCNController::getTCCode(int& tcCode) const {

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

GDataRecord2103 GalilCNController::getStatus() const {

    GDataRecord2103 record;
    writeErrorIfExists(this->getRecord(record));
    return record;

}

void GalilCNController::writeError(int errorCode) const {

    traceErr() << "Galil CN: codice errore:" << errorCode;
    traceErr() << "Galil CN: descrizione errore:" << GalilControllerUtils::getErrorDescription(errorCode);

    if (errorCode == G_BAD_RESPONSE_QUESTION_MARK) {
        int tcCode;
        this->getTCCode(tcCode);
        traceErr() << "Galil CN: dettagli errore:" << GalilControllerUtils::getTCDescription(tcCode);
    };

}

void GalilCNController::writeErrorIfExists(int errorCode) const {

    if (errorCode != G_NO_ERROR)
        writeError(errorCode);

}

int GalilCNController::getInputs(int bank, int& bankStatus) {

    traceEnter;

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

int GalilCNController::tellSwitches(GalilCNController::Axis a, int& value) {

    traceEnter;

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
