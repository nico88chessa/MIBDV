#ifndef ABSTRACTDEVICEINSPECTOR_HPP
#define ABSTRACTDEVICEINSPECTOR_HPP

#include <QObject>
#include <QTimer>

#include <configure.h>
#include <Logger.hpp>

namespace PROGRAM_NAMESPACE {

class AbstractDeviceInspector : public QObject {
    Q_OBJECT

public:
    using Ptr = AbstractDeviceInspector*;
    using ConstPtr = const AbstractDeviceInspector*;

protected:
    QTimer refreshTimer;
    int restartTimeMs;

public:
    static constexpr int DEFAULT_INTERVAL_MS = 1000;
    static constexpr int DEFAULT_RESTART_DELAY_TIME_MS = DEFAULT_INTERVAL_MS;

    explicit AbstractDeviceInspector(QObject *parent = nullptr);
    ~AbstractDeviceInspector();
    void setRefreshValue(int intervalMs = DEFAULT_INTERVAL_MS);
    void setRestartTime(int restartTimeMs = DEFAULT_RESTART_DELAY_TIME_MS);

protected:
    virtual void initializeInspector();
    virtual bool beforeProcess();
    virtual void afterStop();
    virtual void process();
    virtual bool getStatus(QVariant &status) = 0;

public slots:
    void startProcess();
    void stopProcess();
    void restartProcess();

signals:
    void processStartedSignal();
    void processStoppedSignal();
    void statusSignal(const QVariant& status);
    void isRunningSignal(bool isRunning);

};

}

#endif // ABSTRACTDEVICEINSPECTOR_HPP
