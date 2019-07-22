#ifndef MACHINESTATUSHANDLER_HPP
#define MACHINESTATUSHANDLER_HPP

#include <QObject>
#include <QWeakPointer>
#include <QScopedPointer>

#include <configure.h>
#include <Utils.hpp>
#include <Logger.hpp>


namespace PROGRAM_NAMESPACE {


#define DECL_MACHINE_STATUS_RECEIVER(TYPE) \
    QScopedPointer<PROGRAM_NAMESPACE::MachineStatusReceiver> machineStatusR; \
    friend void PROGRAM_NAMESPACE::MachineStatusDispatcher::addReceiverT<TYPE>(TYPE&); \
    template <typename> friend class PROGRAM_NAMESPACE::hasMachineStatusReceiver;


/*
 * M A C H I N E S T A T U S R E C E I V E R
 */

class MachineStatusReceiver : public QObject {
    Q_OBJECT

public:
    using Ptr = MachineStatusReceiver*;
    using ConstPtr = const MachineStatusReceiver*;

private:
    MachineStatus currentStatus;
    const int id;

    static int receiverId;

public:
    explicit MachineStatusReceiver(QObject* parent = Q_NULLPTR);
    int getId() const;

public slots:
    MachineStatus getCurrentStatus() const;
    void setCurrentStatus(const MachineStatus& value);

};



/*
 * M A C H I N E S T A T U S N O T I F I E R
 */

class MachineStatusNotifier : public QObject {
    Q_OBJECT

public:
    using Ptr = MachineStatusNotifier*;
    using ConstPtr = const MachineStatusNotifier*;

private:
    MachineStatus currentStatus;
    const int id;

    static int notifierId;

public:
    explicit MachineStatusNotifier(QObject* parent = Q_NULLPTR);
    int getId() const;


public slots:
    MachineStatus getCurrentStatus() const;
    void setCurrentStatus(const MachineStatus& value);

signals:
    void machineStatusNotifierSignal(MachineStatus status);

};



/*
 * M A C H I N E S T A T U S M A N A G E R
 */

/**
 * @brief The MachineStatusDispatcher class
 * Questa classe fa da dispatcher: l'update dello stato generato dal notifier, viene inviato al dispatcher
 * che lo inoltra ai vari receiver; i receiver sono agganciati usando il meccanismo signal/slot di Qt
 */
class MachineStatusDispatcher : public QObject {
    Q_OBJECT

public:
    using Ptr = MachineStatusDispatcher*;
    using ConstPtr = const MachineStatusDispatcher*;

private:
    QWeakPointer<MachineStatusNotifier> notifier;

    /* NOTE NIC 10/07/2019 - variabile sato macchina
     * questo mi serve per mantenere lo stato della macchina quando non c'e' alcun
     * notifier; inizializzo la macchina a STATUS_NAN
     */
    MachineStatus currentStatus;

public:
    explicit MachineStatusDispatcher(QObject* parent = Q_NULLPTR);

    bool addNotifier(const QSharedPointer<MachineStatusNotifier>& newNotifier);
    void addReceiver(MachineStatusReceiver& newReceiver);

    bool canAddNotifier() const;

    template <typename T>
    void addReceiverT(T& newReceiver) {

        traceEnter;
        static_assert (hasMachineStatusReceiver<T>::value, "Il tipo dell'oggetto non ha un MachineStatusReceiver");
        addReceiver(*newReceiver.machineStatusR);
        traceExit;

    }

    MachineStatus getCurrentStatus() const;

public slots:
    void setCurrentStatus(MachineStatus status);

signals:
    void machineStatusSignal(MachineStatus status);

};


inline static void initializeMachineStatusReceiver(QScopedPointer<MachineStatusReceiver>& es, QObject* parent = Q_NULLPTR) {
    es.reset(new MachineStatusReceiver(parent));
}


template<typename T>
class hasMachineStatusReceiver {
    using yes = char[1];
    using no = char[2];

    // la () alla fine serve per simulare un'istanziazione; se non ci fosse, il trait non funzionerebbe
    template <typename C> static yes& test(decltype(std::declval<C>().machineStatusR)());
    template <typename C> static no&  test(...);
public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};


}

#endif // MACHINESTATUSHANDLER_HPP
