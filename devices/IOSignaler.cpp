#include <QtGlobal>

#include "IOSignaler.hpp"
#include "Settings.hpp"
#include <GalilCNStatusBean.hpp>
#include <GalilPLCStatusBean.hpp>


using namespace PROGRAM_NAMESPACE;

/* NOTE NIC 19/07/2019 - gestione ErrorId per il generic input
 *
 * bit 16 = 1 -> codice dell'applicativo A
 * bit 15 = 1 -> indico un generic input G
 * bit 08 - 14 -> riservato per il deviceKey D
 * bit 0 - 7 -> riservato per il channel del device C
 * quindi: 0000000A GDDDDDDD CCCCCCCC
 */

static constexpr ErrorID IO_SIGNALER_GENERIC_INPUT_MASK = 0x01 << 15;
static constexpr ErrorID IO_SIGNALER_GENERIC_INPUT_DEVICE_KEY_SHIFT_MASK = 8;

static constexpr ErrorID IO_SIGNALER_POWER = PROGRAM_ERR_START_CODE + 1;
static constexpr ErrorID IO_SIGNALER_CYCLE = PROGRAM_ERR_START_CODE + 2;
static constexpr ErrorID IO_SIGNALER_EMERGENCY_MUSHROOM = PROGRAM_ERR_START_CODE + 3;
static constexpr ErrorID IO_SIGNALER_DOOR_OPEN = PROGRAM_ERR_START_CODE + 4;
static constexpr ErrorID IO_SIGNALER_MAINTENANCE = PROGRAM_ERR_START_CODE + 5;
static constexpr ErrorID IO_SIGNALER_WATER_ALARM = PROGRAM_ERR_START_CODE + 6;
static constexpr ErrorID IO_SIGNALER_MARK_IN_PROGRESS = PROGRAM_ERR_START_CODE + 7;
static constexpr ErrorID IO_SIGNALER_SCANNER_READY = PROGRAM_ERR_START_CODE + 8;
static constexpr ErrorID IO_SIGNALER_SCANNER_ERROR = PROGRAM_ERR_START_CODE + 9;
static constexpr ErrorID IO_SIGNALER_DISTANCE_SENSOR_FAULT = PROGRAM_ERR_START_CODE + 10;
static constexpr ErrorID IO_SIGNALER_DEPRESSURE_SENSOR_FAULT = PROGRAM_ERR_START_CODE + 11;
static constexpr ErrorID IO_SIGNALER_PRESSURE_1_SENSOR_FAULT = PROGRAM_ERR_START_CODE + 12;
static constexpr ErrorID IO_SIGNALER_PRESSURE_2_SENSOR_FAULT = PROGRAM_ERR_START_CODE + 13;
static constexpr ErrorID IO_SIGNALER_GENERIC_INPUT = PROGRAM_ERR_START_CODE + IO_SIGNALER_GENERIC_INPUT_MASK;

static constexpr char IO_SIGNALER_POWER_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Power off");
static constexpr char IO_SIGNALER_CYCLE_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Cycle not enabled");
static constexpr char IO_SIGNALER_EMERGENCY_MUSHROOM_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Emergency button pressed");
static constexpr char IO_SIGNALER_DOOR_OPEN_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Door open");
static constexpr char IO_SIGNALER_MAINTENANCE_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Machine is in maintenance mode");
static constexpr char IO_SIGNALER_WATER_ALARM_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Water is low");
static constexpr char IO_SIGNALER_MARK_IN_PROGRESS_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Mark in progress");
static constexpr char IO_SIGNALER_SCANNER_READY_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Scanner ready");
static constexpr char IO_SIGNALER_SCANNER_ERROR_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Scanner error");
static constexpr char IO_SIGNALER_DISTANCE_SENSOR_FAULT_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Distance sensor fault");
static constexpr char IO_SIGNALER_DEPRESSURE_SENSOR_FAULT_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Depressure sensor fault");
static constexpr char IO_SIGNALER_PRESSURE_1_SENSOR_FAULT_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Pressure 1 sensor fault");
static constexpr char IO_SIGNALER_PRESSURE_2_SENSOR_FAULT_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Pressure 2 sensor fault");
static constexpr char IO_SIGNALER_GENERIC_INPUT_DESCR[] = QT_TRANSLATE_NOOP("mibdv" , "Errore input: ");



IOSignaler::IOSignaler(QObject *parent) : QObject(parent),
    digitalInputLastValues(), isFirst(true),
    refreshTimer(this),
    needSignaler(false) {

    const Settings& settings = Settings::instance();
    DigitalInputSet dInMap = settings.getDigitalInputs();
    DigitalOutputSet dOutMap = settings.getDigitalOutputs();
    AnalogInputSet aInMap = settings.getAnalogInputs();

    for (auto&& dIn: dInMap.values()) {
        if (dIn.getChannel() == DIGITAL_INPUT_CHANNEL_NONE)
            continue;
        digitalInputStatus.insertMulti(dIn.getElementType(), dIn);
        digitalInputLastValues.insertMulti(dIn.getElementType(), dIn);
    }

    for (auto&& dOut: dOutMap.values()) {
        if (dOut.getChannel() == DIGITAL_OUTPUT_CHANNEL_NONE)
            continue;
        digitalOutputStatus.insert(dOut.getElementType(), dOut);
    }

    for (auto&& aIn: aInMap.values()) {
        if (aIn.getChannel() == ANALOG_INPUT_CHANNEL_NONE)
            continue;
        analogInputStatus.insertMulti(aIn.getElementType(), aIn);
    }

    int refreshTimeMs = settings.getMachineIORefreshIntervalMs();
    refreshTimer.setInterval(refreshTimeMs);

    initializeErrorSignaler(errorSignaler, this);
    initializeMachineStatusReceiver(machineStatusR, this);

    connect(&refreshTimer, &QTimer::timeout, this, &IOSignaler::process);

}

IOSignaler::~IOSignaler() {

    traceEnter;
    this->stopProcess();
    traceExit;

}

void IOSignaler::analizeIO() {

    /* TODO NIC 18/07/2019 - notifiche I/O
     * sarebbe da distinguere i signal degli I/O;
     * non ha senso emettere un signal se non c'e' stato alcun cambiamento
     * degli I/O (per esempio gli input digitali)
     */

    traceEnter;

    if (isFirst) {
        for (auto&& dIn: digitalInputStatus.values())
            digitalInputLastValues[dIn.getElementType()] = dIn;

        isFirst = false;
    } else {

        auto&& powerValue = digitalInputStatus.value(IOType::POWER).getValue();
        auto&& powerOldValue = digitalInputLastValues.value(IOType::POWER).getValue();
        if (powerValue != powerOldValue) {
            if (powerValue == true)
                emit powerOnSignal();
            else
                emit powerOffSignal();
            digitalInputLastValues[IOType::POWER].setValue(powerValue);
        }

        auto&& cycleValue = digitalInputStatus.value(IOType::CYCLE).getValue();
        auto&& cycleOldValue = digitalInputLastValues.value(IOType::CYCLE).getValue();
        if (cycleValue != cycleOldValue) {
            if (cycleValue == true)
                emit cycleOnSignal();
            else
                emit cycleOffSignal();
            digitalInputLastValues[IOType::CYCLE].setValue(cycleValue);
        }

        auto&& markInProgressValue = digitalInputStatus.value(IOType::MARK_IN_PROGRESS).getValue();
        auto&& markInProgressOldValue = digitalInputLastValues.value(IOType::MARK_IN_PROGRESS).getValue();
        if (markInProgressValue != markInProgressOldValue) {
            if (markInProgressValue == true)
                emit markInProgressOnSignal();
            else
                emit markInProgressOffSignal();
            digitalInputLastValues[IOType::MARK_IN_PROGRESS].setValue(markInProgressValue);
        }

    }

    // qui verifico gli errori
    errorSignaler->clearErrors();

    MachineStatus machineStatus = machineStatusR->getCurrentStatus();

    for (auto&& dIn: digitalInputStatus.values()) {

        if (!dIn.getIsAlarm())
            continue;

        /* NOTE NIC 22/07/2019
         * se alarmInverted = false -> allarme e' attivo con logica alta
         * se alarmInverted = true -> allarme e' attivo con logica bassa
         * la logica dipende dal parametro invertLogic;
         * quindi l'allarme e' attivo quando input e inverted sono uno negato dell'altro
         */
        bool showAlarm = dIn.getValue() == !dIn.getIsAlarmInverted();

        if (showAlarm) {
            ErrorID errorId;
            QString errorDescr;
            bool isAlarm = false;
            switch (dIn.getElementType()) {
            case IOType::POWER:
                errorId = IO_SIGNALER_POWER;
                errorDescr = IO_SIGNALER_POWER_DESCR;
                isAlarm = machineStatus == MachineStatus::PRINTING;
                break;
            case IOType::CYCLE:
                errorId = IO_SIGNALER_CYCLE;
                errorDescr = IO_SIGNALER_CYCLE_DESCR;
                isAlarm = machineStatus == MachineStatus::PRINTING;
                break;
            case IOType::EMERGENCY_MUSHROOM:
                errorId = IO_SIGNALER_EMERGENCY_MUSHROOM;
                errorDescr = IO_SIGNALER_EMERGENCY_MUSHROOM_DESCR;
                isAlarm = true;
                break;
            case IOType::DOOR_OPEN:
                errorId = IO_SIGNALER_DOOR_OPEN;
                errorDescr = IO_SIGNALER_DOOR_OPEN_DESCR;
                isAlarm = !digitalInputStatus.value(IOType::MAINTENANCE).getValue();
                break;
            case IOType::MAINTENANCE:
                errorId = IO_SIGNALER_MAINTENANCE;
                errorDescr = IO_SIGNALER_MAINTENANCE_DESCR;
                isAlarm = false;
                break;
            case IOType::WATER_ALARM:
                errorId = IO_SIGNALER_WATER_ALARM;
                errorDescr = IO_SIGNALER_WATER_ALARM_DESCR;
                isAlarm = true;
                break;
            case IOType::MARK_IN_PROGRESS:
                errorId = IO_SIGNALER_MARK_IN_PROGRESS;
                errorDescr = IO_SIGNALER_MARK_IN_PROGRESS_DESCR;
                isAlarm = false;
                break;
            case IOType::SCANNER_READY:
                errorId = IO_SIGNALER_SCANNER_READY;
                errorDescr = IO_SIGNALER_SCANNER_READY_DESCR;
                isAlarm = false;
                break;
            case IOType::SCANNER_ERROR:
                errorId = IO_SIGNALER_SCANNER_ERROR;
                errorDescr = IO_SIGNALER_SCANNER_ERROR_DESCR;
                isAlarm = true;
                break;
            case IOType::DISTANCE_SENSOR_FAULT:
                errorId = IO_SIGNALER_DISTANCE_SENSOR_FAULT;
                errorDescr = IO_SIGNALER_DISTANCE_SENSOR_FAULT_DESCR;
                isAlarm = true;
                break;
            case IOType::DEPRESSURE_SENSOR_FAULT:
                errorId = IO_SIGNALER_DEPRESSURE_SENSOR_FAULT;
                errorDescr = IO_SIGNALER_DEPRESSURE_SENSOR_FAULT_DESCR;
                isAlarm = true;
                break;
            case IOType::PRESSURE_1_SENSOR_FAULT:
                errorId = IO_SIGNALER_PRESSURE_1_SENSOR_FAULT;
                errorDescr = IO_SIGNALER_PRESSURE_1_SENSOR_FAULT_DESCR;
                isAlarm = true;
                break;
            case IOType::PRESSURE_2_SENSOR_FAULT:
                errorId = IO_SIGNALER_PRESSURE_2_SENSOR_FAULT;
                errorDescr = IO_SIGNALER_PRESSURE_2_SENSOR_FAULT_DESCR;
                isAlarm = true;
                break;
            case IOType::GENERIC_INPUT:
                errorId = buildGenericErrorId(dIn);
                errorDescr = QString("%1%2").arg(IO_SIGNALER_GENERIC_INPUT_DESCR).arg(dIn.getName());
                isAlarm = true;
                break;
            }

//            if (machineStatus == MachineStatus::PRINTING || isAlwaysAlarm)
            errorSignaler->addError(Error(DeviceKey::IO_SIGNALER, errorId, errorDescr, isAlarm ? ErrorType::ERROR : ErrorType::WARNING));
//            else
//                errorSignaler->addError(Error(DeviceKey::IO_SIGNALER, errorId, errorDescr, ErrorType::WARNING));
        }

    }

    errorSignaler->notifyErrors();

    isFirst = false;

    traceExit;

}

ErrorID IOSignaler::buildGenericErrorId(const DigitalInputValue& dIn) const {

    traceEnter;
    traceExit;
    return IO_SIGNALER_GENERIC_INPUT + (static_cast<int>(dIn.getDevice()) << IO_SIGNALER_GENERIC_INPUT_DEVICE_KEY_SHIFT_MASK) + dIn.getChannel();

}

void IOSignaler::process() {

    traceEnter;

    if (this->needSignaler)
        emit statusSignal(digitalInputStatus, digitalOutputStatus, analogInputStatus);

    needSignaler = false;

    traceExit;

}

void IOSignaler::restartProcess() {
    traceEnter;
    // do nothing
    traceExit;
}

void IOSignaler::startProcess() {

    traceEnter;
    refreshTimer.start();
    emit processStartedSignal();
    traceExit;

}

void IOSignaler::stopProcess() {

    traceEnter;

    if (refreshTimer.isActive()) {
        refreshTimer.stop();
        emit processStoppedSignal();
    }

    traceExit;

}

void IOSignaler::updateStatus(const GalilCNStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)

    /* NOTE NIC 18/07/2019 - fungo emergenza
     * il fungo non e' associato ad un IO fisico ma all'ingresso di Abort del CN;
     * sono costretto a gestirlo a parte
     */
    for (auto&& dInKey: digitalInputStatus.keys()) {

        /* NOTE NIC 19/07/2019 - gestione generic input
         * avendo gli ingressi generici (per scopi futuri), potrebbe capitare
         * di avere la stessa chiave GENERIC_INPUT per piu' valori;
         * per prima cosa, controllo in che situazione sono
         */
        if (digitalInputStatus.values(dInKey).size() == 1) {

            // ho solo un valore associato alla chiave, allora prendo il valore per reference
            auto&& dIn = digitalInputStatus[dInKey];
            bool value;
            bool isAssigned = false;

            if (dIn.getDevice() == DeviceKey::GALIL_CN) {

                int channel = dIn.getChannel();
                if (channel == DIGITAL_INPUT_CHANNEL_VIRTUAL) {

                    switch (dIn.getElementType()) {
                    case IOType::EMERGENCY_MUSHROOM: value = status.getIsEmergencyMushroomPressed(); isAssigned = true; break;
                    }

                } else {
                    value = status.getDigitalInput(channel);
                    isAssigned = true;
                }

                if (!isAssigned)
                    continue;

                if (dIn.getInvertLogic())
                    value = !value;
                dIn.setValue(value);
            }

        } else {

            // ho piu' valori associati alla stessa chiave; allora itero i valori (per reference)
            auto&& disIt = digitalInputStatus.find(dInKey);
            while (disIt != digitalInputStatus.end() && disIt.key() == dInKey) {
                auto&& dIn = disIt.value();
                if (dIn.getDevice() == DeviceKey::GALIL_CN) {
                    bool value = status.getDigitalInput(dIn.getChannel());
                    if (dIn.getInvertLogic())
                        value = !value;
                    dIn.setValue(value);
                }
                ++disIt;
            }

        }
    }

    for (auto&& dOutKey: digitalOutputStatus.keys()) {
        auto& dOut = digitalOutputStatus[dOutKey];
        if (dOut.getDevice() == DeviceKey::GALIL_CN) {
            bool value = status.getDigitalOutput(dOut.getChannel());
            if (dOut.getInvertLogic())
                value = !value;
            dOut.setValue(value);
        }
    }

    this->analizeIO();
    needSignaler = true;
    traceExit;

}

void IOSignaler::updateStatus(const GalilPLCStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)
    for (auto&& dInKey: digitalInputStatus.keys()) {

        if (digitalInputStatus.values(dInKey).size() == 1) {

            auto&& dIn = digitalInputStatus[dInKey];
            bool value;
            bool isAssigned = false;

            if (dIn.getDevice() == DeviceKey::GALIL_PLC) {

                int channel = dIn.getChannel();
                if (channel == DIGITAL_INPUT_CHANNEL_VIRTUAL) {
                    // NOTE NIC 19/07/2019 - niente da fare per ora
                } else {
                    value = status.getDigitalInput(channel);
                    isAssigned = true;
                }

                if (!isAssigned)
                    continue;

                if (dIn.getInvertLogic())
                    value = !value;
                dIn.setValue(value);
            }

        } else {

            auto&& i = digitalInputStatus.find(dInKey);
            while (i != digitalInputStatus.end() && i.key() == dInKey) {
                auto&& dIn = i.value();
                if (dIn.getDevice() == DeviceKey::GALIL_PLC) {
                    bool value = status.getDigitalInput(dIn.getChannel());
                    if (dIn.getInvertLogic())
                        value = !value;
                    dIn.setValue(value);
                }
                ++i;
            }

        }
    }

    for (auto&& dOutKey: digitalOutputStatus.keys()) {
        auto& dOut = digitalOutputStatus[dOutKey];
        if (dOut.getDevice() == DeviceKey::GALIL_PLC) {
            bool value = status.getDigitalOutput(dOut.getChannel());
            if (dOut.getInvertLogic())
                value = !value;
            dOut.setValue(value);
        }
    }


    for (auto&& aInKey: analogInputStatus.keys()) {

        AnalogInputStatus::iterator aisIt = analogInputStatus.find(aInKey);
        while (aisIt != analogInputStatus.end() && aisIt.key() == aInKey) {
            auto& aIn = aisIt.value();
            if (aIn.getDevice() == DeviceKey::GALIL_PLC) {
                analogReal value = status.getAnalogInput(aIn.getChannel());
                value = value / GALIL_PLC_ANALOG_COUNT_MAX_VALUE * aIn.getGain() + aIn.getOffset();
                aIn.setValue(value);
            }
            ++aisIt;
        }
    //
    //    QList<AnalogInputValue>&& values = analogInputStatus.values(aInKey);
    //    for (auto&& aIn: values) {
    //        if (aIn.getDevice() == DeviceKey::GALIL_PLC) {
    //            analogReal value = status.getAnalogInput(aIn.getChannel());
    //            value = value / GALIL_PLC_ANALOG_COUNT_MAX_VALUE * aIn.getGain() + aIn.getOffset();
    //            aIn.setValue(value);
    //        }
    //    }
    }

    this->analizeIO();
    needSignaler = true;
    traceExit;

}
