#include "GalilCNController.hpp"

using namespace PROGRAM_NAMESPACE;

void GalilCNController::writeError(int errorCode) {

    traceErr() << "Descrizione errore:" << GalilCNControllerUtils::getErrorString(errorCode);

}

bool GalilCNController::getInputs(int bank, int& bankStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Il controller non e' stato inizializzato";
        return false;
    }

    if (bank<0 || bank>6) {
        traceErr() << "La funzione get inputs accetta un valore compreso fra 0 e 6";
        return false;
    }

    QString command = "TI " + QString::number(bank);
    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmdI(handle(), command.toStdString().c_str(), &bankStatus);

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilCNController::tellSwitched(GalilCNController::Axis a, int& value) {

    traceEnter;

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = "TS" + axisLetter;

    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmdI(handle(), command.toStdString().data(), &value);

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

    return result == G_NO_ERROR;

}

GalilCNController::GalilCNController() :
    handler(nullptr), numDigitalInput(0), numDigitalOutput(0) {

    traceEnter;
    traceExit;

}

GalilCNController::~GalilCNController() {

    traceEnter;

    GReturn result = G_NO_ERROR;

    if (!this->handler.isNull())
        result = GClose(handle());

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

}

void GalilCNController::setupController(int numDigitalInput, int numDigitalOutput, int numAnalogInput) {

    traceEnter;

    if (isInitialized)
        return;

    this->numDigitalInput = numDigitalInput;
    this->numDigitalOutput = numDigitalOutput;
    this->numAnalogInput = numAnalogInput;

    traceExit;

}

bool GalilCNController::connect(const QString& ip) {

    traceEnter;

    if (!this->handler.isNull())
        return true;
    else
        handler.reset(new GCon);

//    QString str = ip + " -d";
    GReturn result = GOpen(ip.toStdString().data(), handler.data());
//    GReturn result = GOpen(str.toStdString().data(), handle.data());

    /**
     * NOTE NIC 2018/10/18
     * se non riesco a connettermi al Galil, allora devo resettare il puntatore;
     * altrimenti nel distruttore viene lanciata un'eccezione
     **/

    if (result != G_NO_ERROR) {
        handler.reset();
        writeError(result);
    }

    traceExit;

    return result == G_NO_ERROR;;

}

bool GalilCNController::getRecord(GDataRecord2103& record) {

    traceEnter;

    GDataRecord recordUnion;
    GReturn result = GRecord(handle(), &recordUnion, G_QR);
    record = recordUnion.dmc2103;

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilCNController::getDigitalInput(int input, int& inputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Il controller non e' stato inizializzato";
        return false;
    }

    if (input<1 || input>numDigitalInput) {
        traceErr() << "Input richiesto fuori dal range del device";
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

bool GalilCNController::getDigitalOutput(int output, int& outputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Il controller non e' stato inizializzato";
        return false;
    }

    if (output<1 || output>numDigitalOutput) {
        traceErr() << "Output richiesto fuori dal range del device";
        return false;
    }

    QString command = QString("MG @OUT[%1]").arg(output);
    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmdI(handle(), command.toStdString().data(), &outputStatus);

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilCNController::getAnalogInput(int analogInput, int& analogInputStatus) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Il controller non e' stato inizializzato";
        return false;
    }

    if (analogInput<1 || analogInput>numAnalogInput) {
        traceErr() << "Output richiesto fuori dal range del device";
        return false;
    }

    QString command = QString("MG @AN[%1]").arg(analogInput);
    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmdI(handle(), command.toStdString().data(), &analogInputStatus);

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilCNController::getPosition(GalilCNController::Axis a, GalilCNController::positionType& pos) {

    traceEnter;

    QString axisLetter(GalilCNController::letterFromAxis(a));
    QString command = "MG _MT" + axisLetter;
    int motorType;

    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmdI(handle(), command.toStdString().data(), &motorType);

    if (result != G_NO_ERROR){
        writeError(result);
        return false;
    }

    command.clear();

    if (qAbs(motorType)==1)
        command = "TP" + axisLetter; // servo motor
    else
        command = "RP" + axisLetter; // step motor

    traceDebug() << "Invio comando:" << command;
    result = GCmdD(handle(), command.toStdString().data(), &pos);

    if (result != G_NO_ERROR)
        writeError(result);

    traceDebug() << "Posizione asse" << axisLetter << ":" << pos;

    traceExit;

    return result == G_NO_ERROR;

}

bool GalilCNController::isAxisInMotion(GalilCNController::Axis a, bool& inMotion) {

    traceEnter;

    int value = 0;
    if (!tellSwitched(a, value))
        return false;

    inMotion = value & (0x01 << static_cast<int>(SwitchBitMask::AXIS_IN_MOTION));

    traceExit;

    return true;

}

bool GalilCNController::isAxisPositionError(GalilCNController::Axis a, bool& isPositionError) {

    traceEnter;

    int value = 0;
    if (!tellSwitched(a, value))
        return false;

    isPositionError = value & (0x01 << static_cast<int>(SwitchBitMask::POSITION_ERROR_EXCEEDS_ERROR_LIMIT));

    traceExit;

    return true;

}

bool GalilCNController::isMotorOff(GalilCNController::Axis a, bool& isMotorOff) {

    traceEnter;

    int value = 0;
    if (!tellSwitched(a, value))
        return false;

    isMotorOff = value & (0x01 << static_cast<int>(SwitchBitMask::MOTOR_OFF));

    traceExit;

    return true;

}

bool GalilCNController::isForwardLimit(GalilCNController::Axis a, bool& isForwardLimit) {

    traceEnter;

    int value = 0;
    if (!tellSwitched(a, value))
        return false;

    isForwardLimit = value & (0x01 << static_cast<int>(SwitchBitMask::FORWARD_LIMIT_SWITCH_INACTIVE));

    traceExit;

    return true;

}

bool GalilCNController::isBackwardLimit(GalilCNController::Axis a, bool& isForwardLimit) {

    traceEnter;

    int value = 0;
    if (!tellSwitched(a, value))
        return false;

    isForwardLimit = value & (0x01 << static_cast<int>(SwitchBitMask::FORWARD_LIMIT_SWITCH_INACTIVE));

    traceExit;

    return true;

}

bool GalilCNController::isHomeAxis(GalilCNController::Axis a, bool& isHome) {

    traceEnter;

    int value = 0;
    if (!tellSwitched(a, value))
        return false;

    isHome = value & (0x01 << static_cast<int>(SwitchBitMask::HOME_SWITCH_STATUS));

    traceExit;

    return true;

}

bool GalilCNController::checkAbort(bool& isAbort) {

    traceEnter;

    QString command = "MG _AB";
    int value;
    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmdI(handle(), command.toStdString().data(), &value);


    if (result != G_NO_ERROR)
        writeError(result);

    isAbort = value == 1;

    traceExit;
    return result == G_NO_ERROR;

}

bool GalilCNController::setDigitalOutput(int output, bool value) {

    traceEnter;

    if (!isInitialized) {
        traceErr() << "Il controller non e' stato inizializzato";
        return false;
    }

    if (output<1 || output>numDigitalOutput) {
        traceErr() << "Input richiesto fuori dal range del device";
        return false;
    }

    QString command = value ? "SB " : "CB ";
    command.append(QString::number(output));

    traceDebug() << "Invio comando:" << command;
    GReturn result = GCmd(handle(), command.toStdString().data());

    if (result != G_NO_ERROR)
        writeError(result);

    traceExit;
    return result == G_NO_ERROR;

}
