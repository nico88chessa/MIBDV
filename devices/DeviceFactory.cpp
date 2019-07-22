#include "DeviceFactory.hpp"
#include "MotionManagerImpl.hpp"
#include "IOManager.hpp"

#include <QtCore>
#include <QObject>
#include <QApplication>

#include <common/NamedThread.hpp>
#include <common/MachineStatusHandler.hpp>
#include <Settings.hpp>
#include <Logger.hpp>

#include <devices/galil/GalilCNInspector.hpp>
#include <devices/galil/GalilPLCInspector.hpp>
#include <devices/IOSignaler.hpp>
#include <devices/MotionAnalizerFactory.hpp>
#include <devices/DeviceConnectionWatcher.hpp>


using namespace PROGRAM_NAMESPACE;

static constexpr const char* GALIL_CN_INSPECTOR_THREAD_NAME = "GALIL_CN_INSPECTOR_THREAD";
static constexpr const char* GALIL_PLC_INSPECTOR_THREAD_NAME = "GALIL_PLC_INSPECTOR_THREAD";
static constexpr const char* IO_SIGNALER_THREAD_NAME = "IO_SIGNALER_THREAD";


DeviceFactory::DeviceFactory() {

    traceEnter;

    errorManager.reset(new ErrorManager());
    machineStatusDispatcher.reset(new MachineStatusDispatcher());
    this->initMotionAnalizer();
    this->initIOSignaler();

    traceExit;

}

void DeviceFactory::stop() {

    traceEnter;

    Settings& s = Settings::instance();

    this->ioSignaler->unsubscribeAll();

    QMetaObject::invokeMethod(ioSignaler.data(), "stopProcess", Qt::QueuedConnection);
    ioSignaler->thread()->wait();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN)
        QMetaObject::invokeMethod(galilCNInspector.data(), "stopProcess", Qt::QueuedConnection);
    galilCNInspector->thread()->wait();

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC)
        QMetaObject::invokeMethod(galilPLCInspector.data(), "stopProcess", Qt::QueuedConnection);
    galilPLCInspector->thread()->wait();


    traceExit;

}

DeviceFactory::~DeviceFactory() { }

void DeviceFactory::initMotionAnalizer() {

    traceEnter;

    Settings& s = Settings::instance();

    if (this->motionAnalizer.isNull()) {


        if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

            if (galilCNInspector.isNull()) {

                galilCNInspector.reset(new GalilCNInspector());

                NamedThread* thread = new NamedThread(GALIL_CN_INSPECTOR_THREAD_NAME);
                QObject::connect(thread, &QThread::started, galilCNInspector.data(), &GalilCNInspector::startProcess);
                QObject::connect(galilCNInspector.data(), &GalilCNInspector::processStoppedSignal, [thread]() {
                    thread->quit();
                });
                /* NOTE NIC 31/05/2019 - non devo assolutamente associare il segnale di finished al deleteLater;
                 * usando i QSharedPointer, si occupa il QSharedPointer stesso a eliminare l'oggetto una volta deferenziato
                 *
                 * QObject::connect(thread, &QThread::finished, galilCNInspector.data(), &GalilCNInspector::deleteLater);
                 */
                QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

                galilCNInspector.data()->moveToThread(thread);
                galilCNInspector->thread()->start();

            }
            this->getErrorManager().data()->subscribeObject(*galilCNInspector.dynamicCast<GalilCNInspector>().data());

            motionAnalizer.reset(MotionAnalizerFactory::createAnalizer(galilCNInspector.dynamicCast<GalilCNInspector>()));
            this->getErrorManager().data()->subscribeObject(*motionAnalizer);
            this->machineStatusDispatcher->addReceiverT(*motionAnalizer);
//            motionAnalizer.reset(MotionAnalizerFactory::createAnalizer(galilCNInspector.dynamicCast<GalilPLCInspector>()));
//            motionAnalizer.reset(new GalilCNMotionAnalizer());
//            motionSignaler->subscribeInspector(galilCNInspector.dynamicCast<GalilCNInspector>());

        }

    }

    traceExit;

}

void DeviceFactory::initIOSignaler() {

    traceEnter;

    Settings& s = Settings::instance();

    if (this->ioSignaler.isNull()) {

        ioSignaler.reset(new IOSignaler());

        if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

            if (galilCNInspector.isNull()) {

                galilCNInspector.reset(new GalilCNInspector());

                QThread* thread = new NamedThread(GALIL_CN_INSPECTOR_THREAD_NAME);
                QObject::connect(thread, &QThread::started, galilCNInspector.data(), &AbstractConnectedDeviceInspector::startProcess);
                QObject::connect(galilCNInspector.data(), &AbstractConnectedDeviceInspector::processStoppedSignal, [thread]() {
                    thread->quit();
                });
                /* NOTE NIC 31/05/2019 - non devo assolutamente associare il segnale di finished al deleteLater;
                 * usando i QSharedPointer, si occupa il QSharedPointer stesso a eliminare l'oggetto una volta deferenziato
                 *
                 * QObject::connect(thread, &QThread::finished, galilCNInspector.data(), &AbstractConnectedDeviceInspector::deleteLater);
                 */
                QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

                galilCNInspector.data()->moveToThread(thread);
                galilCNInspector->thread()->start();

            }

            this->getErrorManager().data()->subscribeObject(*galilCNInspector.dynamicCast<GalilCNInspector>().data());
            ioSignaler->subscribeInspector(galilCNInspector.dynamicCast<GalilCNInspector>());

        }

        if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {

            if (galilPLCInspector.isNull()) {

                galilPLCInspector.reset(new GalilPLCInspector());

                QThread* thread = new NamedThread(GALIL_PLC_INSPECTOR_THREAD_NAME);
                QObject::connect(thread, &QThread::started, galilPLCInspector.data(), &AbstractConnectedDeviceInspector::startProcess);
                QObject::connect(galilPLCInspector.data(), &AbstractConnectedDeviceInspector::processStoppedSignal, [thread]() {
                    thread->quit();
                });
                /* NOTE NIC 31/05/2019 - non devo assolutamente associare il segnale di finished al deleteLater;
                 * usando i QSharedPointer, si occupa il QSharedPointer stesso a eliminare l'oggetto una volta deferenziato
                 *
                 * QObject::connect(thread, &QThread::finished, galilPLCInspector.data(), &AbstractConnectedDeviceInspector::deleteLater);
                 */
                QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

                galilPLCInspector.data()->moveToThread(thread);
                galilPLCInspector->thread()->start();

            }

            this->getErrorManager().data()->subscribeObject(*galilPLCInspector.dynamicCast<GalilPLCInspector>().data());
            ioSignaler->subscribeInspector(galilPLCInspector.dynamicCast<GalilPLCInspector>());

        }

        QThread* ioSignalerThread = new NamedThread(IO_SIGNALER_THREAD_NAME);
        QObject::connect(ioSignalerThread, &QThread::started, ioSignaler.data(), &IOSignaler::startProcess);
        QObject::connect(ioSignaler.data(), &IOSignaler::processStoppedSignal, [ioSignalerThread]() {
            ioSignalerThread->quit();
        });
        /* NOTE NIC 31/05/2019 - non devo assolutamente associare il segnale di finished al deleteLater;
         * usando i QSharedPointer, si occupa il QSharedPointer stesso a eliminare l'oggetto una volta deferenziato
         *
         * QObject::connect(ioSignalerThread, &QThread::finished, ioSignaler.data(), &IOSignaler::deleteLater);
         */
        QObject::connect(ioSignalerThread, &QThread::finished, ioSignalerThread, &QThread::deleteLater);

        this->errorManager->subscribeObject(*ioSignaler.data());
        this->machineStatusDispatcher->addReceiverT(*ioSignaler.data());

        ioSignaler.data()->moveToThread(ioSignalerThread);
        ioSignaler->thread()->start();

    }

    traceExit;

}

bool DeviceFactory::existsEventLoop() const {

    traceEnter;
    bool exists = QThread::currentThread() != nullptr && QThread::currentThread()->eventDispatcher() != nullptr;
    traceExit;
    return exists;

}

DeviceFactory& DeviceFactory::instance() {

    static DeviceFactory df;
    return df;

}

QWeakPointer<IMotionAnalizer> DeviceFactory::getMotionAnalizer() {

    traceEnter;
    if (motionAnalizer.isNull())
        initMotionAnalizer();
    traceExit;
    return motionAnalizer.toWeakRef();

}

QWeakPointer<IOSignaler> DeviceFactory::getIOSignaler() {
    traceEnter;
    if (ioSignaler.isNull())
        initIOSignaler();
    traceExit;
    return ioSignaler.toWeakRef();
}

QSharedPointer<MotionManager> DeviceFactory::instanceMotionManager() {

    traceEnter;
    ThreadName currentThreadName = QThread::currentThread()->objectName();

    if (motionManagers.contains(currentThreadName))
        return motionManagers.value(currentThreadName);

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        if (!galilCNControllers.contains(currentThreadName)) {
            QSharedPointer<GalilCNController> galilController(new GalilCNController());
            galilController->setupController(
                        s.getGalilCNIpAddress(),
                        s.getGalilCNConnectionTimeoutMs(),
                        s.getGalilCNNumberDigitalInput(),
                        s.getGalilCNNumberDigitalOutput(),
                        s.getGalilCNNumberAnalogInput(),
                        s.getGalilCNOptionCustomHomeAxisX());
            galilCNControllers.insert(currentThreadName, galilController);

        }

        auto&& galilController = galilCNControllers.value(currentThreadName);

        DeviceConnectionWatcherKey key(currentThreadName, DeviceKey::GALIL_CN);

//        if (!galilCNConnectionWatchers.contains(currentThreadName)) {
        if (!connectionWatchers.contains(key)) {
            if (existsEventLoop()) {
                int id = connectionWatchers.size() + 1;
                QSharedPointer<DeviceConnectionWatcher> galilCNConnectionWatcher(new DeviceConnectionWatcher(id));
                galilCNConnectionWatcher->setDevice(galilController.toWeakRef());
                galilCNConnectionWatcher->setupTimers(s.getGalilCNCheckConnectionIntervalMs());

                connectionWatchers.insert(key, galilCNConnectionWatcher);
                this->getErrorManager().data()->subscribeObject(*galilCNConnectionWatcher.data());

                galilCNConnectionWatcher->startWatcher();
            }
        }

        QSharedPointer<MotionManager> motionManager(new MotionManagerImpl<GalilCNController>(galilController.staticCast<GalilCNController>()));

        motionManagers.insert(currentThreadName, motionManager);

    }

    traceExit;
    return motionManagers.value(currentThreadName);

}

QSharedPointer<IOManager> DeviceFactory::instanceIOManager() {

    traceEnter;
    ThreadName currentThreadName = QThread::currentThread()->objectName();

    if (ioManagers.contains(currentThreadName))
        return ioManagers.value(currentThreadName);

    Settings& s = Settings::instance();

    QSharedPointer<IOManager> ioManager(new IOManager());
    ioManagers.insert(currentThreadName, ioManager);

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        if (!galilCNControllers.contains(currentThreadName)) {
            QSharedPointer<GalilCNController> galilController(new GalilCNController());
            galilController->setupController(
                        s.getGalilCNIpAddress(),
                        s.getGalilCNConnectionTimeoutMs(),
                        s.getGalilCNNumberDigitalInput(),
                        s.getGalilCNNumberDigitalOutput(),
                        s.getGalilCNNumberAnalogInput(),
                        s.getGalilCNOptionCustomHomeAxisX());
            galilCNControllers.insert(currentThreadName, galilController);
        }

        auto&& galilController = galilCNControllers.value(currentThreadName);

        DeviceConnectionWatcherKey key(currentThreadName, DeviceKey::GALIL_CN);
        if (!connectionWatchers.contains(key)) {
            if (existsEventLoop()) {
                int id = connectionWatchers.size() + 1;
                QSharedPointer<DeviceConnectionWatcher> galilCNConnectionWatcher(new DeviceConnectionWatcher(id));
                galilCNConnectionWatcher->setDevice(galilController.toWeakRef());
                galilCNConnectionWatcher->setupTimers(s.getGalilCNCheckConnectionIntervalMs());

                connectionWatchers.insert(key, galilCNConnectionWatcher);
                this->getErrorManager().data()->subscribeObject(*galilCNConnectionWatcher.data());

                galilCNConnectionWatcher->startWatcher();
            }
        }

        ioManager->addDevice(DeviceKey::GALIL_CN, galilController.staticCast<GalilCNController>());

    }

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {

        if (!galilPLCControllers.contains(currentThreadName)) {
            QSharedPointer<GalilPLCController> galilController(new GalilPLCController());
            galilController->setupController(
                        s.getGalilPLCIpAddress(),
                        s.getGalilPLCConnectionTimeoutMs(),
                        s.getGalilPLCNumberDigitalInput(),
                        s.getGalilPLCNumberDigitalOutput(),
                        s.getGalilPLCNumberAnalogInput());
            galilPLCControllers.insert(currentThreadName, galilController);
        }

        auto&& galilController = galilPLCControllers.value(currentThreadName);

        DeviceConnectionWatcherKey key(currentThreadName, DeviceKey::GALIL_PLC);
        if (!connectionWatchers.contains(key)) {
            if (existsEventLoop()) {
                int id = connectionWatchers.size() + 1;
                QSharedPointer<DeviceConnectionWatcher> galilPLCConnectionWatcher(new DeviceConnectionWatcher(id));
                galilPLCConnectionWatcher->setDevice(galilController.toWeakRef());
                galilPLCConnectionWatcher->setupTimers(s.getGalilPLCCheckConnectionIntervalMs());

                connectionWatchers.insert(key, galilPLCConnectionWatcher);
                this->getErrorManager().data()->subscribeObject(*galilPLCConnectionWatcher.data());

                galilPLCConnectionWatcher->startWatcher();
            }
        }

        ioManager->addDevice(DeviceKey::GALIL_PLC, galilController.staticCast<GalilPLCController>());

    }

    traceExit;
    return ioManager;

}

QWeakPointer<ErrorManager> DeviceFactory::getErrorManager() {

    traceEnter;
    return this->errorManager.toWeakRef();
    traceExit;

}

QSharedPointer<MachineStatusNotifier> DeviceFactory::instanceMachineStatusNotifier(QObject* parent) const {

    traceEnter;
    if (!this->machineStatusDispatcher->canAddNotifier()) {
        traceWarn() << "E' gia presente un Machine status Notifier";
        return QSharedPointer<MachineStatusNotifier>();
    }
    QSharedPointer<MachineStatusNotifier> msn(new MachineStatusNotifier(parent));
    machineStatusDispatcher->addNotifier(msn);
    traceExit;
    return msn;

}

void DeviceFactory::detachManagers() {

    traceEnter;
    ThreadName currentThreadName = QThread::currentThread()->objectName();

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {
        galilCNControllers.remove(currentThreadName);
        DeviceConnectionWatcherKey key(currentThreadName, DeviceKey::GALIL_CN);
        if (connectionWatchers.contains(key)) {
            connectionWatchers.value(key)->stopWatcher();
            connectionWatchers.remove(key);
        }

    }
    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {
        galilPLCControllers.remove(currentThreadName);
        DeviceConnectionWatcherKey key(currentThreadName, DeviceKey::GALIL_PLC);
        if (connectionWatchers.contains(key)) {
            connectionWatchers.value(key)->stopWatcher();
            connectionWatchers.remove(key);
        }
    }

    motionManagers.remove(currentThreadName);
    ioManagers.remove(currentThreadName);

    traceExit;

}
