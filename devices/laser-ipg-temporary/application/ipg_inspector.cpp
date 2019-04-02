#include "ipg_inspector.h"

using namespace ipg;

IpgInspector::IpgInspector(QObject* parent) :
    QObject(parent),
    laserAsyncInterface(this),
    refreshTimer(this), standbyTimer(this) {

    /**
     * NOTE NIC 11/09/2018: agli oggetti timer e laserAsyncInterface va specificato che il padre e' l'istanza di questa classe;
     * questo perche' nel caso di operazione come moveToThread, il thread deve spostare l'oggetto e tutti i suoi figli.
     */
    connect(&laserAsyncInterface, SIGNAL(connectedSignal()), this, SIGNAL(connectedSignal()));
    connect(&laserAsyncInterface, SIGNAL(disconnectedSignal()), this, SLOT(handleDisconnection()));
    connect(&laserAsyncInterface, SIGNAL(getLaserStatusSignal(ipg::GetLaserStatusOutput)), this, SLOT(handleStatus(ipg::GetLaserStatusOutput)));

    /**
     * NOTE NIC 01/10/2018: standbyTimer serve ad indicare il massimo intervallo di tempo in cui non ho ricevuto alcuna informazione
     * dal laser; se entro tale tempo non ricevo nulla, chiudo la connessione.
     * (questa implementazione supera il bug delle qt tale per cui, sconnettendo il cavo ethernet, il socket non si accorge della
     * mancanza di connessione (QTBUG-57548)
     */
    connect(&standbyTimer, SIGNAL(timeout()), this, SLOT(handleDisconnection()));

    connect(this, SIGNAL(disconnectedSignal()), this, SLOT(restartProcess()));
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(process()));

    refreshTimer.setInterval(REFRESH_TIME_MS_DFLT);
    standbyTimer.setInterval(STANDBY_MAX_TIME_MS_DFLT);

}

void IpgInspector::setAddress(const QString& ip, quint16 port) {

    this->ip = ip;
    this->port = port;

}

void IpgInspector::setRefreshTime(int refreshTime) {

    refreshTimer.setInterval(refreshTime);

}

void IpgInspector::setStandbyTime(int standbyMaxTime) {

    int refreshTimerInterval = refreshTimer.interval();

    if (standbyMaxTime < refreshTimerInterval) {
        qDebug() << "  - Ipg inspector: standby time troppo basso; lo imposto a 5 volte il tempo di refresh.";
        standbyMaxTime = refreshTimerInterval * 5;
    }

    standbyTimer.setInterval(standbyMaxTime);

}

IpgInspector::~IpgInspector() {

    qDebug() << "  - Ipg inspector: in eliminazione.";
    refreshTimer.stop();
    standbyTimer.stop();
    qDebug() << "  - Ipg inspector: timer fermato.";
    laserAsyncInterface.closeConnection();
    qDebug() << "  - Ipg inspector: eliminato.";

}

void IpgInspector::process() {

    if (!laserAsyncInterface.getLaserStatus())
        qDebug() << "  - Ipg inspector: errore nella chiamata tcp per avere lo stato";

}

void IpgInspector::handleStatus(ipg::GetLaserStatusOutput status) {

    standbyTimer.start();
    emit laserStatusSignal(status);

}

void IpgInspector::handleDisconnection() {

    standbyTimer.stop();
    refreshTimer.stop();

    /**
     * NOTE NIC: devo sconnettere il segnale, altrimenti il segnale disconnectSignal viene generato due volte:
     * 1. quando chiamo il metodo closeConnection()
     * 2. quanto emetto il segnale alla fine di questo metodo
     */
    disconnect(&laserAsyncInterface, SIGNAL(disconnectedSignal()), this, SLOT(handleDisconnection()));
    laserAsyncInterface.closeConnection();
    connect(&laserAsyncInterface, SIGNAL(disconnectedSignal()), this, SLOT(handleDisconnection()));
    emit disconnectedSignal();

}

void IpgInspector::restartProcess() {

    if (standbyTimer.isActive())
        standbyTimer.stop();
    if (refreshTimer.isActive())
        refreshTimer.stop();

    QTimer::singleShot(RECONNECTION_TIME_MS_DFLT, this, SLOT(startProcess()));

}

void IpgInspector::startProcess() {

    if (!laserAsyncInterface.connectToLaser(ip, port)) {
        QTimer::singleShot(RECONNECTION_TIME_MS_DFLT, this, SLOT(startProcess()));
        return;
    }

    refreshTimer.start();
    standbyTimer.start();

    emit processStartSignal();

}

void IpgInspector::stopProcess() {

    qDebug() << "  - Ipg inspector: fermo il processo.";
    disconnect(this, SIGNAL(disconnectedSignal()), this, SLOT(restartProcess()));

    if (standbyTimer.isActive())
        standbyTimer.stop();

    if (refreshTimer.isActive())
        refreshTimer.stop();

    laserAsyncInterface.closeConnection();

    emit processStopSignal();

}

