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

/* TODO NIC 28/05/2019 - iniziare a gestire l'error manager
 * qui andrebbe agganciata la logica dell'error manager a vari inspector
 */

class DeviceFactory {
public:
    using Ptr = DeviceFactory*;
    using ConstPtr = const DeviceFactory*;

    using ThreadName = QString;

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

    QMap<ThreadName, QSharedPointer<DeviceConnectionWatcher>> galilCNConnectionWatchers;
    QMap<ThreadName, QSharedPointer<DeviceConnectionWatcher>> galilPLCConnectionWatchers;

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

//    MachineStatusDispatcher* getMachineStatusDispatcher() const;
};

}

#ifndef DeviceFactoryInstance
#define DeviceFactoryInstance PROGRAM_NAMESPACE::DeviceFactory::instance()
#endif

#endif // DEVICEFACTORY_HPP
