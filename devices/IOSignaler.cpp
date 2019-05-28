#include "IOSignaler.hpp"
#include "Settings.hpp"
#include <GalilCNStatusBean.hpp>
#include <GalilPLCStatusBean.hpp>


using namespace PROGRAM_NAMESPACE;

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
        digitalInputStatus.insert(dIn.getElementType(), dIn);
        digitalInputLastValues.insert(dIn.getElementType(), dIn);
    }

    for (auto&& dOut: dOutMap.values()) {
        if (dOut.getChannel() == DIGITAL_OUTPUT_CHANNEL_NONE)
            continue;
        digitalOutputStatus.insert(dOut.getElementType(), dOut);
    }

    for (auto&& aIn: aInMap.values()) {
        if (aIn.getChannel() == ANALOG_INPUT_CHANNEL_NONE)
            continue;
        analogInputStatus.insert(aIn.getElementType(), aIn);
    }

    int refreshTimeMs = settings.getMachineIORefreshIntervalMs();
    refreshTimer.setInterval(refreshTimeMs);

    connect(&refreshTimer, &QTimer::timeout, this, &IOSignaler::process);

}

void IOSignaler::analizeIO() {

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

    traceExit;

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
    emit processStartSignal();
    traceExit;

}

void IOSignaler::stopProcess() {

    traceEnter;

    if (refreshTimer.isActive())
        refreshTimer.stop();

    emit processStopSignal();
    traceExit;

}

void IOSignaler::updateStatus(const GalilCNStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)
    for (auto&& dInKey: digitalInputStatus.keys()) {
        auto& dIn = digitalInputStatus[dInKey];
        if (dIn.getDevice() == DeviceKey::GALIL_CN) {
            bool value = status.getDigitalInput(dIn.getChannel());
            if (dIn.getInvertLogic())
                value = !value;
            dIn.setValue(value);
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
        auto& dIn = digitalInputStatus[dInKey];
        if (dIn.getDevice() == DeviceKey::GALIL_PLC) {
            bool value = status.getDigitalInput(dIn.getChannel());
            if (dIn.getInvertLogic())
                value = !value;
            dIn.setValue(value);
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
        auto& aIn = analogInputStatus[aInKey];
        if (aIn.getDevice() == DeviceKey::GALIL_PLC) {
            analogReal value = status.getAnalogInput(aIn.getChannel());
            value = value / GALIL_PLC_ANALOG_COUNT_MAX_VALUE * aIn.getGain() + aIn.getOffset();
            aIn.setValue(value);
        }
    }

    this->analizeIO();
    needSignaler = true;
    traceExit;

}
