#ifndef DEVICEFACTORY_HPP
#define DEVICEFACTORY_HPP

#include <QMap>
#include <QScopedPointer>
#include <QSharedPointer>

#include <configure.h>
#include <ErrorHandler.hpp>


namespace PROGRAM_NAMESPACE {

class GalilPLCController;
class GalilCNController;
class AbstractConnectedDeviceInspector;
class IMotionAnalizer;
class IOSignaler;
class MotionManager;
class IOManager;
class DeviceConnectionWatcher;
class MachineStatusDispatcher;
class MachineStatusNotifier;


class DeviceFactory {
public:
    using Ptr = DeviceFactory*;
    using ConstPtr = const DeviceFactory*;

    using ThreadName = QString;

    // utilizzo una classe interna
    class DeviceConnectionWatcherKey {
    private:
        QString threadName;
        DeviceKey deviceId;

    public:
        DeviceConnectionWatcherKey(const QString& threadName, DeviceKey id) :
            threadName(threadName), deviceId(id) { }
        DeviceKey getDeviceId() const { return deviceId; }
        void setDeviceId(const DeviceKey& value) { deviceId = value; }
        QString getThreadName() const { return threadName; }
        void setThreadName(const QString& value) { threadName = value; }

        // TODO NIC 17/07/2019 - da verificare funzionamento
        friend inline bool operator<(const DeviceConnectionWatcherKey& l, const DeviceConnectionWatcherKey& r);

    };

private:
    DeviceFactory();
    ~DeviceFactory();

    // errorManager
    QSharedPointer<ErrorManager> errorManager;

    // machine state dispatcher
    QScopedPointer<MachineStatusDispatcher> machineStatusDispatcher;

    // oggetti per la gestione degli inspector
    QSharedPointer<AbstractConnectedDeviceInspector> galilPLCInspector;
    QSharedPointer<AbstractConnectedDeviceInspector> galilCNInspector;
    QSharedPointer<IMotionAnalizer> motionAnalizer;
    QSharedPointer<IOSignaler> ioSignaler;


    /* NOTE NIC 28/05/2019 - inizio la gestione dei vari controller; per evitare problemi di concorrenza,
     * a ciascun thread (identificato dal nome), viene allocato il controller richiesto;
     * se il controllo non esiste, viene istanziato, altrimenti viene restituito quello gia'
     * esistente
     */

    // lista di controller veri e propri
    QMap<ThreadName, QSharedPointer<GalilCNController>> galilCNControllers;
    QMap<ThreadName, QSharedPointer<GalilPLCController>> galilPLCControllers;

    // lista di connection watchers
    QMap<DeviceConnectionWatcherKey, QSharedPointer<DeviceConnectionWatcher>> connectionWatchers;

    // wrapper che mascherano l'implementazione dei controller
    QMap<ThreadName, QSharedPointer<MotionManager>> motionManagers;
    QMap<ThreadName, QSharedPointer<IOManager>> ioManagers;

private:
    void initMotionAnalizer();
    void initIOSignaler();

    /**
     * @brief existsEventLoop
     * @return true se nel thread corrente esiste un EventLoop
     */
    bool existsEventLoop() const;

public:
    void stop();
    DeviceFactory(const DeviceFactory& df) = delete;
    void operator=(const DeviceFactory& df) = delete;

    static DeviceFactory& instance();

    QWeakPointer<IMotionAnalizer> getMotionAnalizer();
    QWeakPointer<IOSignaler> getIOSignaler();

    QSharedPointer<MotionManager> instanceMotionManager();
    QSharedPointer<IOManager> instanceIOManager();

    QWeakPointer<ErrorManager> getErrorManager();

    QSharedPointer<MachineStatusNotifier> instanceMachineStatusNotifier(QObject* parent = Q_NULLPTR) const;

    // qui rimuove i managers / controller dal thread corrente
    void detachManagers();

};

bool operator<(const DeviceFactory::DeviceConnectionWatcherKey& l, const DeviceFactory::DeviceConnectionWatcherKey& r) {
    if (l.threadName < r.threadName)
        return true;
    return static_cast<int>(l.deviceId) < static_cast<int>(r.deviceId);
//    return static_cast<int>(l.deviceId) < static_cast<int>(r.deviceId) && (l.threadName < r.threadName);
}

}

#ifndef DeviceFactoryInstance
#define DeviceFactoryInstance PROGRAM_NAMESPACE::DeviceFactory::instance()
#endif

#endif // DEVICEFACTORY_HPP
