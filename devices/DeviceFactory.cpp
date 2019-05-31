#include "DeviceFactory.hpp"
#include "MotionManagerImpl.hpp"
#include "IOManager.hpp"

#include <QtCore>
#include <QObject>
#include <QApplication>

#include <common/NamedThread.hpp>
#include <Settings.hpp>
#include <Logger.hpp>

#include <devices/galil/GalilCNInspector.hpp>
#include <devices/galil/GalilPLCInspector.hpp>
#include <devices/IOSignaler.hpp>
#include <devices/MotionSignaler.hpp>
#include <devices/InspectorStatusNotifier.hpp>
#include <devices/DeviceConnectionWatcher.hpp>

using namespace PROGRAM_NAMESPACE;

static constexpr const char* GALIL_CN_INSPECTOR_THREAD_NAME = "GALIL_CN_INSPECTOR_THREAD";
static constexpr const char* GALIL_PLC_INSPECTOR_THREAD_NAME = "GALIL_PLC_INSPECTOR_THREAD";
static constexpr const char* IO_SIGNALER_THREAD_NAME = "IO_SIGNALER_THREAD";

DeviceFactory::DeviceFactory() {

    traceEnter;

    inspectorStatusNotifier.reset(new InspectorStatusNotifier());
    errorManager.reset(new ErrorManager());
    this->initMotionSignaler();
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
    galilCNInspector->thread();

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC)
        QMetaObject::invokeMethod(galilPLCInspector.data(), "stopProcess", Qt::QueuedConnection);
    galilPLCInspector->thread()->wait();


    traceExit;

}

DeviceFactory::~DeviceFactory() { }

void DeviceFactory::initMotionSignaler() {

    traceEnter;

    Settings& s = Settings::instance();

    if (this->motionSignaler.isNull()) {

        motionSignaler.reset(new MotionSignaler());

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
                QObject::connect(galilCNInspector.data(), &AbstractDeviceInspector::isRunningSignal,
                                 inspectorStatusNotifier.data(), &InspectorStatusNotifier::galilCNInspectorIsRunningSignal);

                galilCNInspector.data()->moveToThread(thread);
                galilCNInspector->thread()->start();

            }
            motionSignaler->subscribeInspector(galilCNInspector.dynamicCast<GalilCNInspector>());

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
                QObject::connect(galilCNInspector.data(), &AbstractDeviceInspector::isRunningSignal,
                                 inspectorStatusNotifier.data(), &InspectorStatusNotifier::galilCNInspectorIsRunningSignal);

                galilCNInspector.data()->moveToThread(thread);
                galilCNInspector->thread()->start();

            }

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
                QObject::connect(galilPLCInspector.data(), &AbstractDeviceInspector::isRunningSignal,
                                 inspectorStatusNotifier.data(), &InspectorStatusNotifier::galilPLCInspectorIsRunningSignal);

                galilPLCInspector.data()->moveToThread(thread);
                galilPLCInspector->thread()->start();

            }

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

InspectorStatusNotifier* DeviceFactory::getInspectorStatusNotifier() const {
    traceEnter;
    return this->inspectorStatusNotifier.data();
    traceExit;
}

QWeakPointer<MotionSignaler> DeviceFactory::getMotionSignaler() {

    traceEnter;
    if (motionSignaler.isNull())
        initMotionSignaler();
    traceExit;
    return motionSignaler.toWeakRef();

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

        if (!galilCNConnectionWatchers.contains(currentThreadName)) {
            if (existsEventLoop()) {
                QSharedPointer<DeviceConnectionWatcher> galilCNConnectionWatcher(new DeviceConnectionWatcher());
                galilCNConnectionWatcher->setDevice(galilController.toWeakRef());
                galilCNConnectionWatcher->setupTimers(s.getGalilCNCheckConnectionIntervalMs());
                galilCNConnectionWatchers.insert(currentThreadName, galilCNConnectionWatcher);
                galilCNConnectionWatcher->startWatcher();
            }
        }

        QSharedPointer<MotionManager> motionManager(new MotionManagerImpl<GalilCNController>(galilController.staticCast<GalilCNController>()));

        // segnali asse x
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisXMotorOffSignal, motionManager.data(), &MotionManager::axisXMotorOffSignal);
        QObject::connect(motionSignaler.data(), static_cast<void (MotionSignaler::*)(MotionStopCode)>(&MotionSignaler::axisXMotionStopSignal),
                         motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisXMotionStopSignal));
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisXForwardLimitSignal, motionManager.data(), &MotionManager::axisXForwardLimitSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisXBackwardLimitSignal, motionManager.data(), &MotionManager::axisXBackwardLimitSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisXHomeInProgressStartSignal, motionManager.data(), &MotionManager::axisXHomeInProgressStartSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisXHomeInProgressStopSignal, motionManager.data(), &MotionManager::axisXHomeInProgressStopSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisXHomingComplete, motionManager.data(), &MotionManager::axisXHomingComplete);

        // segnali asse y
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisYMotorOffSignal, motionManager.data(), &MotionManager::axisYMotorOffSignal);
        QObject::connect(motionSignaler.data(), static_cast<void (MotionSignaler::*)(MotionStopCode)>(&MotionSignaler::axisYMotionStopSignal),
                         motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisYMotionStopSignal));
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisYForwardLimitSignal, motionManager.data(), &MotionManager::axisYForwardLimitSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisYBackwardLimitSignal, motionManager.data(), &MotionManager::axisYBackwardLimitSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisYHomeInProgressStartSignal, motionManager.data(), &MotionManager::axisYHomeInProgressStartSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisYHomeInProgressStopSignal, motionManager.data(), &MotionManager::axisYHomeInProgressStopSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisYHomingComplete, motionManager.data(), &MotionManager::axisYHomingComplete);

        // segnali asse z
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisZMotorOffSignal, motionManager.data(), &MotionManager::axisZMotorOffSignal);
        QObject::connect(motionSignaler.data(), static_cast<void (MotionSignaler::*)(MotionStopCode)>(&MotionSignaler::axisZMotionStopSignal),
                         motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisZMotionStopSignal));
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisZForwardLimitSignal, motionManager.data(), &MotionManager::axisZForwardLimitSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisZBackwardLimitSignal, motionManager.data(), &MotionManager::axisZBackwardLimitSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisZHomeInProgressStartSignal, motionManager.data(), &MotionManager::axisZHomeInProgressStartSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisZHomeInProgressStopSignal, motionManager.data(), &MotionManager::axisZHomeInProgressStopSignal);
        QObject::connect(motionSignaler.data(), &MotionSignaler::axisZHomingComplete, motionManager.data(), &MotionManager::axisZHomingComplete);

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

        if (!galilCNConnectionWatchers.contains(currentThreadName)) {
            if (existsEventLoop()) {
                QSharedPointer<DeviceConnectionWatcher> galilCNConnectionWatcher(new DeviceConnectionWatcher());
                galilCNConnectionWatcher->setDevice(galilController.toWeakRef());
                galilCNConnectionWatcher->setupTimers(s.getGalilCNCheckConnectionIntervalMs());
                galilCNConnectionWatchers.insert(currentThreadName, galilCNConnectionWatcher);
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

        if (!galilPLCConnectionWatchers.contains(currentThreadName)) {
            if (existsEventLoop()) {
                QSharedPointer<DeviceConnectionWatcher> galilPLCConnectionWatcher(new DeviceConnectionWatcher());
                galilPLCConnectionWatcher->setDevice(galilController.toWeakRef());
                galilPLCConnectionWatcher->setupTimers(s.getGalilPLCCheckConnectionIntervalMs());
                galilPLCConnectionWatchers.insert(currentThreadName, galilPLCConnectionWatcher);
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

void DeviceFactory::detachManagers() {

    traceEnter;
    ThreadName currentThreadName = QThread::currentThread()->objectName();

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {
        galilCNControllers.remove(currentThreadName);
        if (galilCNConnectionWatchers.contains(currentThreadName)) {
            galilCNConnectionWatchers.value(currentThreadName)->stopWatcher();
            galilCNConnectionWatchers.remove(currentThreadName);
        }

    }
    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {
        galilPLCControllers.remove(currentThreadName);
        if (galilPLCConnectionWatchers.contains(currentThreadName)) {
            galilPLCConnectionWatchers.value(currentThreadName)->stopWatcher();
            galilPLCConnectionWatchers.remove(currentThreadName);
        }
    }

    motionManagers.remove(currentThreadName);
    ioManagers.remove(currentThreadName);

    traceExit;

}
