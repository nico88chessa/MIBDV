#ifndef GALILPLCINSPECTOR_HPP
#define GALILPLCINSPECTOR_HPP

#include <QObject>
#include <QTimer>
#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <galil/GalilPLCController.hpp>
#include <ErrorManager.hpp>

namespace PROGRAM_NAMESPACE {

class GalilPLCInspector : public QObject {
    Q_OBJECT

public:
    using Ptr = GalilPLCInspector*;
    using ConstPtr = GalilPLCInspector*;

private:
    int reconnectionIntervalMs;
    QString ipAddress;

    QScopedPointer<GalilPLCController> controller;
    QTimer refreshTimer;
    DECL_ERROR_SIGNALER_FRIENDS(GalilPLCInspector)

public:
    explicit GalilPLCInspector(QObject* parent = nullptr);
    ~GalilPLCInspector();

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
    void statusSignal(GalilPLCStatusBean);

    void processStartSignal();
    void processStopSignal();

protected:

};

}

#endif // GALILPLCINSPECTOR_HPP

