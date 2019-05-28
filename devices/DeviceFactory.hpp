#ifndef DEVICEFACTORY_HPP
#define DEVICEFACTORY_HPP

#include <QMap>
#include <QScopedPointer>
#include <QSharedPointer>

#include <configure.h>


namespace PROGRAM_NAMESPACE {

class GalilPLCController;
class GalilCNController;
class AbstractConnectedDeviceInspector;
class MotionSignaler;
class IOSignaler;
class MotionManager;
class IOManager;
class InspectorStatusNotifier;

/* TODO NIC 28/05/2019 - iniziare a gestire l'error manager
 * qui andrebbe agganciata la logica dell'error manager a vari inspector
 */

class DeviceFactory {
public:
    using Ptr = DeviceFactory*;
    using ConstPtr = const DeviceFactory*;

private:
    DeviceFactory();
    ~DeviceFactory();

    // questo serve per controllare se gli inspector dei dispositivi funzionano
    QScopedPointer<InspectorStatusNotifier> inspectorStatusNotifier;

    // oggetti per la gestione degli inspector
    QSharedPointer<AbstractConnectedDeviceInspector> galilPLCInspector;
    QSharedPointer<AbstractConnectedDeviceInspector> galilCNInspector;
    QSharedPointer<MotionSignaler> motionSignaler;
    QSharedPointer<IOSignaler> ioSignaler;


    /* NOTE NIC 28/05/2019 - inizio la gestione dei vari controller; per evitare problemi di concorrenza,
     * a ciascun thread (identificato dal nome), viene allocato il controller richiesto;
     * se il controllo non esiste, viene istanziato, altrimenti viene restituito quello gia'
     * esistente
     */

    // lista di controller veri e propri
    QMap<QString, QSharedPointer<GalilCNController>> galilCNControllers;
    QMap<QString, QSharedPointer<GalilPLCController>> galilPLCControllers;

    // wrapper che mascherano l'implementazione dei controller
    QMap<QString, QSharedPointer<MotionManager>> motionManagers;
    QMap<QString, QSharedPointer<IOManager>> ioManagers;

private:
    void initMotionSignaler();
    void initIOSignaler();

public:
    DeviceFactory(const DeviceFactory& df) = delete;
    void operator=(const DeviceFactory& df) = delete;

    static DeviceFactory& instance();

    InspectorStatusNotifier* getInspectorStatusNotifier() const;

    QWeakPointer<MotionSignaler> getMotionSignaler();
    QWeakPointer<IOSignaler> getIOSignaler();

    QSharedPointer<MotionManager> instanceMotionManager();
    QSharedPointer<IOManager> instanceIOManager();

    // qui rimuove i managers / controller dal thread corrente
    void detachManagers();

};

}

#ifndef DeviceFactoryInstance
#define DeviceFactoryInstance PROGRAM_NAMESPACE::DeviceFactory::instance()
#endif


#endif // DEVICEFACTORY_HPP
