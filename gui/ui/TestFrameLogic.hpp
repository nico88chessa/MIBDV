#ifndef TESTFRAMELOGIC_HPP
#define TESTFRAMELOGIC_HPP

#include <QObject>

#include <NamedThread.hpp>
#include <MotionManager.hpp>
#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <MachineStatusHandler.hpp>
#include <Error.hpp>
#include <core/image-processor/AbstractProcessor.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>
#include <third-party/ipg-marking-library-wrapper/include/Scanner.h>


class TestFrame;

enum class PointShapeEnum;
class PrintConfiguration;
class MachineStatusReceiver;

enum class PrintCommandExecuted : int {
    IDLE,
    START,
    CYCLE,
    PROCESSOR_THREAD_RUN,
    IO_ACTIVATED,
    LASER_ON,
    SCANNER_HEAD_SETUP,
    PRINT_LOOP
};

class Worker : public QObject {
    Q_OBJECT

public:
    using Ptr = Worker*;
    using ConstPtr = const Worker*;

    static constexpr unsigned int WAIT_THREAD_STARTED_TIME_MS = 10000;
    static constexpr unsigned int MOTION_CHECK_TIME_MS = 200;
    static constexpr unsigned int COUNTDOWN_INTERVAL_MS = 1000;
    static constexpr unsigned int MAX_COUNTDOWN_TICKS = 10;

    // questo parametro serve per far si che lo stato della testa vada a BUSY;
    // infatti se faccio un close() poco dopo aver fatto un open(), il close viene ignorato
    static constexpr int TEST_FRAME_SCANNER_WAIT_TIME_AFTER_CONNECTION_MS = 2000;

    friend class TestFrameLogic;

private:
    TestFrameLogic* tfl;
    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;
    QSharedPointer<PROGRAM_NAMESPACE::MachineStatusNotifier> machineStatusNotifier;
    QScopedPointer<PROGRAM_NAMESPACE::ProcessorThread> fileProcessorThread;
    QScopedPointer<ipg_marking_library_wrapper::Scanner> scanner;
    PrintConfiguration printConfiguration;

    bool hasToStop;
    bool hasErrors;
    QList<PROGRAM_NAMESPACE::Error> errorList;

    PrintCommandExecuted commandsExecuted;

public:
    explicit Worker(QObject* parent = nullptr);

    const PrintConfiguration& getPrintConfiguration() const { return printConfiguration; }
    void setPrintConfiguration(const PrintConfiguration& value) { printConfiguration = value; }

private:
    void setupSignalsAndSlots();
    inline void updateLastCommandExecute(PrintCommandExecuted c) { this->commandsExecuted = c; }

public slots:
    void startProcess();

private:
    bool beforeProcess();
    bool process();
    bool afterProcess();
    inline void showDialogAsync(const QString& err, const QString& descr);
    inline void updateStatusAsync(const QString& status);
    inline void updateTileTimeAsync(unsigned int tileTimeMs);
    inline void updateStackedTimeAsync(unsigned int stackedTimeMs);
    inline void updateEstimatedResidualTimeAsync(unsigned int stackedTimeMs);

    bool setupLaserOn();
    bool setupLaserOff();
    bool getPulseEnergy(float& energyJoule);

signals:
    void finished();
    void stopRequest();

    /* NOTE NIC 25/07/2019 - gestione segnali errori
     * sono obbligato ad usare altri segnali perche' nel processo di stampa, non e' detto che sia
     * sempre in attesa di eventi esterni (quando chiamo processEvents()); allora, per evitare una perdita di un errore,
     * imposto la variabile di istanza hasErrors attraverso uno slot fatto ad-hoc (vedere metodo setupSignalsAndSlots)
     */
    void hasErrorsSignal(QList<PROGRAM_NAMESPACE::Error> errors);
    void hasFatalsSignal(QList<PROGRAM_NAMESPACE::Error> fatals);
    void errorsSignal(); // uso private alla classe Worker

};

class TestFrameLogic : public QObject {
    Q_OBJECT

    friend class TestFrame;
    friend class Worker;

public:
    using Ptr = TestFrameLogic *;
    using ConstPtr = const TestFrameLogic *;

private:
    TestFrame* qPtr;
    NamedThread* workerThread;
    QScopedPointer<PROGRAM_NAMESPACE::MachineStatusReceiver> machineStatusReceiver;

    bool isProcessStopped;
    bool isLaserInitialized;

public:
    explicit TestFrameLogic(QObject* parent = Q_NULLPTR);
    ~TestFrameLogic();

private slots:
    void startWork();
    void stopWork();

    void changeGuideLaserState();
    void updateStatus(const QString& status);
    void updateTileTime(unsigned int tileTimeMs);
    void updateStackedTime(unsigned int stackedTimeMs);
    void updateEstimatedResidualTime(unsigned int stackedTimeMs);

    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)
    void initIpgYLPNLaser();

signals:
    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)
    void laserIpgYLPNinitializedSignal();
    void stopRequest();

};

#endif // TESTFRAMELOGIC_HPP
