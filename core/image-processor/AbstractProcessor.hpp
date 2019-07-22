#ifndef ABSTRACTPROCESSOR_HPP
#define ABSTRACTPROCESSOR_HPP

#include <type_traits>

#include <common/NamedThread.hpp>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

#include <configure.h>
#include <Constants.hpp>


namespace PROGRAM_NAMESPACE {

using ProcessErrorCode = int;

static constexpr ProcessErrorCode PROCESSOR_THREAD_NO_ERROR = 0;
static constexpr ProcessErrorCode PROCESSOR_THREAD_INITIALIZE_ERROR = PROGRAM_ERR_START_CODE + 1;
static constexpr ProcessErrorCode PROCESSOR_THREAD_STOP_REQUEST = PROGRAM_ERR_START_CODE + 2;
static constexpr ProcessErrorCode PROCESSOR_THREAD_END_CODE = PROGRAM_ERR_START_CODE + 10;


/* TODO NIC 14/05/2019 - non sarebbe corretto gestire la condizione di dati full in questa classe,
 * perche' questa classe non dovrebbe sapere come funzione il riempimento dei dati.
 * sarebbe da fare una classe LimitMemoryProcessorThread che gestisca questo concetto usando la condizione
 * dataFullCondition
 */
class ProcessorThread : public NamedThread {
    Q_OBJECT

public:
    using Ptr = ProcessorThread*;
    using ConstPtr = const ProcessorThread*;

private:
    ProcessErrorCode processErrorCode;

protected:
    mutable QMutex mutex;
    mutable QWaitCondition pauseCondition;
    mutable QWaitCondition resultsReadyCondition;
    mutable QWaitCondition spaceAvailableCondition;

    bool pauseRequest;
    bool stopRequest;
    bool isRunningState;

public:

    ProcessorThread(const QString& threadName, QObject *parent = Q_NULLPTR) :
        NamedThread(threadName, parent),
        pauseRequest(false), stopRequest(false), isRunningState(false),
        processErrorCode(PROCESSOR_THREAD_NO_ERROR) { }

    bool isProcessEndingCorrectly(ProcessErrorCode& errorCode) const {
        QMutexLocker locker(&mutex);
        errorCode = this->processErrorCode;
        return errorCode != PROCESSOR_THREAD_NO_ERROR;
    }

    ProcessErrorCode getProcessErrorCode() const {
        QMutexLocker locker(&mutex);
        return this->processErrorCode;
    }

    bool getNext(void* data) {

        QMutexLocker locker(&mutex);

        while (!stopRequest && isRunningState && !isResultsReady())
            resultsReadyCondition.wait(locker.mutex());

        // se ho ricevuto una richiesta di stop, esco
        if (stopRequest)
            return false;

        if (!isRunningState && !isResultsReady())
            return false;

        getNextImpl(data);

        spaceAvailableCondition.notify_all();

        return true;
    }

    bool hasNext() const {

        QMutexLocker locker(&mutex);

        // se il thread e' in esecuzione, o i dati sono pronti oppure lo saranno
        if (this->isRunningState)
            return true;

        if (this->stopRequest)
            return false;

        /* qui il thread non e' in running:
         * 1. o ha finito correttamente, pero' ci sono ancora dati da prelevare
         * 2. o e' stato interrotto
         */

        bool isRR = isResultsReady();
        return isRR;

    }

    void pause() {
        QMutexLocker locker(&mutex);
        this->pauseRequest = true;
    }

    void resume() {
        QMutexLocker locker(&mutex);
        this->pauseRequest = false;
        this->pauseCondition.notify_all();
    }

    void stop() {
        QMutexLocker locker(&mutex);
        this->stopRequest = true;
        this->pauseCondition.notify_all();
        this->spaceAvailableCondition.notify_all();
        this->resultsReadyCondition.notify_all();
    }

protected:

    virtual bool isResultsReady() const = 0;

    void setProcessErrorCode(const ProcessErrorCode& value) {
        QMutexLocker locker(&mutex);
        this->processErrorCode = value;
    }

    void setIsRunning(bool value) {
        QMutexLocker locker(&mutex);
        this->isRunningState = value;
    }

    bool isRunning() const {
        QMutexLocker locker(&mutex);
        return this->isRunningState;
    }

    void run() {

        this->setProcessErrorCode(PROCESSOR_THREAD_NO_ERROR);
        this->setIsRunning(true);
        emit processStarted();

        if (!initialize()) {
            this->setProcessErrorCode(PROCESSOR_THREAD_INITIALIZE_ERROR);
            emit processFinished();
            this->setIsRunning(false);
            return;
        }

        emit loopRunning();
        ProcessErrorCode processErrorCodeLoop = loopProcess();
        this->setProcessErrorCode(processErrorCodeLoop);

        emit processFinished();

        this->setIsRunning(false);

    }

protected:

    virtual ProcessErrorCode loopProcess() = 0;

    virtual bool initialize() = 0;

    virtual void getNextImpl(void* data) = 0;

    virtual bool isSpaceAvailable() const = 0;

signals:
    void processStarted();
    void loopRunning();
    void processFinished();

};


template <typename R>
class AbstractProcessor : public ProcessorThread {
public:
    using Ptr = AbstractProcessor*;
    using ConstPtr = const AbstractProcessor*;

    using resultType = R;

public:
    AbstractProcessor(const QString& threadName, QObject* parent = Q_NULLPTR) :
        ProcessorThread(threadName, parent) { }

protected:
    virtual void getNextImpl(void* data) final override {
        R* dataR = static_cast<R*>(data);
        getNextImpl(*dataR);
    }

    virtual void getNextImpl(R& data) = 0;

};

}


/* NOTE NIC 13/05/2019 - questo e' inserito nella versione di c++ 17;
 * per ora la implemento a mano
 */
template <typename... Ts> using void_t = void;


template <typename T, typename = void>
struct isAbstractProcessor {
    static constexpr bool value = false;
};

template <typename T>
struct isAbstractProcessor<
        T,
        void_t<typename T::resultType> > {
    static constexpr bool value = true;
    using resultType = typename T::resultType;
};


#endif // ABSTRACTPROCESSOR_HPP
