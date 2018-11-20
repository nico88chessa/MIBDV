#ifndef GALILCNINSPECTOR_HPP
#define GALILCNINSPECTOR_HPP

#include <QObject>
#include <QTimer>
#include <QString>
#include <QScopedPointer>

#include <Constants.hpp>
#include <configure.h>
#include <Logger.hpp>
#include <GalilCNController.hpp>
#include <GalilCNStatusBean.hpp>
#include <GalilPLCController.hpp>
#include <GalilPLCStatusBean.hpp>
#include <ErrorSignaler.hpp>
#include <ErrorManager.hpp>
#include <Settings.hpp>

namespace PROGRAM_NAMESPACE {

class GalilCNInspector : public QObject {
    Q_OBJECT

public:
    using Ptr = GalilCNInspector*;
    using ConstPtr = GalilCNInspector*;

private:
    int reconnectionIntervalMs;
    QString ipAddress;

    QScopedPointer<GalilCNController> controller;
    QTimer refreshTimer;
    DECL_ERROR_SIGNALER_FRIENDS(GalilCNInspector)

public:
    explicit GalilCNInspector(QObject* parent = nullptr);
    ~GalilCNInspector();

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

