#ifndef GALILCNINSPECTOR_HPP
#define GALILCNINSPECTOR_HPP

#include <QTimer>
#include <QString>
#include <QScopedPointer>


#include <Constants.hpp>
#include <Logger.hpp>
#include <configure.h>
#include <MotionInspector.hpp>
#include <galil/GalilCNController.hpp>
#include <GalilCNStatusBean.hpp>
#include <ErrorSignaler.hpp>
#include <ErrorManager.hpp>
#include <Settings.hpp>

namespace PROGRAM_NAMESPACE {

class GalilCNInspector : public MotionInspector {
    Q_OBJECT

public:
    using Ptr = GalilCNInspector*;
    using ConstPtr = GalilCNInspector*;

private:
    int reconnectionIntervalMs;
    QString ipAddress;
    GalilCNStatusBean lastStatus;

    QScopedPointer<GalilCNController> controller;
    QTimer refreshTimer;
    DECL_ERROR_SIGNALER_FRIENDS(GalilCNInspector)

    bool isFirst;
    DigitalInput powerInput;
    DigitalInput cycleInput;

public:
    explicit GalilCNInspector(QObject* parent = nullptr);
    ~GalilCNInspector();

private:
    void analizeLastStatus(const GalilCNStatusBean& newStatus);

private slots:
    void process();
    void handleDisconnection();

public slots:
    void restartProcess();
    void startProcess();
    void stopProcess();

signals:
    void connectedSignal();
    void disconnectedSignal();
    void statusSignal(GalilCNStatusBean);

    void processStartSignal();
    void processStopSignal();

protected:

};

}

#endif // GALILCNINSPECTOR_HPP

