#include "MachineStatusHandler.hpp"

using namespace PROGRAM_NAMESPACE;

int MachineStatusReceiver::receiverId = 0;
int MachineStatusNotifier::notifierId = 0;


/*
 * M A C H I N E S T A T U S R E C E I V E R
 */

MachineStatusReceiver::MachineStatusReceiver(QObject* parent) :
    QObject(parent), id(++receiverId) { }

int MachineStatusReceiver::getId() const { return id; }

MachineStatus MachineStatusReceiver::getCurrentStatus() const {
    return currentStatus;
}

void MachineStatusReceiver::setCurrentStatus(const MachineStatus& value) {
    currentStatus = value;
}



/*
 * M A C H I N E S T A T U S N O T I F I E R
 */

MachineStatusNotifier::MachineStatusNotifier(QObject* parent) : QObject(parent), id(++notifierId) { }

int MachineStatusNotifier::getId() const { return id; }

MachineStatus MachineStatusNotifier::getCurrentStatus() const { return currentStatus; }

void MachineStatusNotifier::setCurrentStatus(const MachineStatus& value) {
    currentStatus = value;
    emit machineStatusNotifierSignal(currentStatus);
}



/*
 * M A C H I N E S T A T U S D I S P A T C H E R
 */

MachineStatusDispatcher::MachineStatusDispatcher(QObject* parent) :
    QObject(parent), currentStatus(MachineStatus::IDLE) { }

MachineStatus MachineStatusDispatcher::getCurrentStatus() const { return currentStatus; }

void MachineStatusDispatcher::setCurrentStatus(MachineStatus status) {
    this->currentStatus = status;
    emit machineStatusSignal(currentStatus);
}

bool MachineStatusDispatcher::addNotifier(const QSharedPointer<MachineStatusNotifier>& newNotifier) {

    if (!canAddNotifier())
        return false;
    notifier = newNotifier.toWeakRef();

    connect(notifier.data(), &MachineStatusNotifier::machineStatusNotifierSignal,
            this, &MachineStatusDispatcher::setCurrentStatus, Qt::AutoConnection);
    connect(notifier.data(), &MachineStatusNotifier::destroyed, [&]() {
        this->notifier.clear();
    });
    return true;

}

void MachineStatusDispatcher::addReceiver(const MachineStatusReceiver& newReceiver) {

    /* NOTE NIC 18/07/2019 - AutoConnection
     * non servirebbe autoConnection come parametro perche' e' di default,
     * tuttavia lo metto per questione di chiarezze; la cosa importante e'
     * che lo slot associato al signal deve essere eseguito all'interno del thread in cui l'oggetto
     * dello slot vive (altrimenti ci possono essere problemi di concorrenza)
     */
    connect(this, &MachineStatusDispatcher::machineStatusSignal,
            &newReceiver, &MachineStatusReceiver::setCurrentStatus, Qt::AutoConnection);

}

bool MachineStatusDispatcher::canAddNotifier() const {

    return notifier.isNull();

}
