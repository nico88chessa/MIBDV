#ifndef SOCKET_SYNC_EXECUTOR_HPP
#define SOCKET_SYNC_EXECUTOR_HPP

#include <QTcpSocket>
#include <QTimer>

#include "AbstractSyncExecutor.hpp"
#include "Constants.hpp"

namespace communication {

namespace ethernet {

static const int SOCKET_SYNC_EXECUTOR_SOCKET_CONNECTION_ERROR = 1;
static const int SOCKET_SYNC_EXECUTOR_WRITE_TIMEOUT_ERROR = 2;
static const int SOCKET_SYNC_EXECUTOR_READ_TIMEOUT_ERROR = 3;

class SocketSyncExecutor : public AbstractSyncExecutor {
    Q_OBJECT

public:
    typedef SocketSyncExecutor* Ptr;
    typedef const SocketSyncExecutor* ConstPtr;

private:
    static const int SOCKET_IDLE_MAX_TIME_MS = 10000;

    QString ip;
    quint16 port;
    QTcpSocket socket;

    /**
     * timer utilizzato per chiudere il socket automaticamente nel caso in cui
     * non ricevo piu' dati per un intervallo di tempo; questo e' un
     * workaround al fatto che i socket in qt non 'vedono' la perdita
     * di connessione;
     * il timer lo faccio partire alla connessione con il socket e lo faccio
     * ripartire all'invio e ricezione di ciascun pacchetto
     */
    QTimer standbyTimer;

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);

protected:

    virtual int syncCaller(const QByteArray& bytesIn, QByteArray& bytesOut) {

        if (!(QTcpSocket::ConnectedState == socket.state())) {
            qWarning() << "  - SocketSyncExecutor: socket connection KO";
            return SOCKET_SYNC_EXECUTOR_SOCKET_CONNECTION_ERROR;
        }

        socket.write(bytesIn);

        if (!socket.waitForBytesWritten(CONNECTION_MAX_WRITE_TIME_MS)) {
            qWarning() << "  - SocketSyncExecutor: timeout invio dati";
            return SOCKET_SYNC_EXECUTOR_WRITE_TIMEOUT_ERROR;
        }

        if (!socket.waitForReadyRead(CONNECTION_MAX_READ_TIME_MS)) {
            qWarning() << "  - SocketSyncExecutor: timeout ricezione dati";
            return SOCKET_SYNC_EXECUTOR_READ_TIMEOUT_ERROR;
        }

        bytesOut = socket.readAll();

        standbyTimer.start();

        return 0;

    }

public:

    SocketSyncExecutor(QObject* parent = nullptr) :
        AbstractSyncExecutor(parent),
        ip(CONNECTION_DEFAULT_IP), port(CONNECTION_DEFAULT_PORT),
        socket(this), standbyTimer(this) {

        connect(&socket, SIGNAL(connected()), this, SIGNAL(connected()));
        connect(&socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
        connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)));

        standbyTimer.setInterval(SOCKET_IDLE_MAX_TIME_MS);
        connect(&standbyTimer, SIGNAL(timeout()), this, SLOT(closeConnection()));

    }

    SocketSyncExecutor(const QString& ipAddress, quint16 portNumber, QObject* parent = nullptr) :
        AbstractSyncExecutor(parent),
        ip(ipAddress), port(portNumber) { }

    ~SocketSyncExecutor() {

        if (QTcpSocket::ConnectedState == socket.state())
            socket.close();

    }

    void setupConnection(QString ipAddress, quint16 portNumber) {

        if (QTcpSocket::UnconnectedState == socket.state()) {

            this->ip = ipAddress;
            this->port = portNumber;

        } else
            qWarning() << "  - SocketSyncExecutor: socket gia connesso all'host" << this->ip << ":" << this->port;

    }

    bool createConnection() {

        socket.connectToHost(this->ip, this->port);

        if (!socket.waitForConnected(CONNECTION_DEFAULT_TIMEOUT_MS)) {
            qDebug() << " - SocketSyncExecutor: connection refused nel metodo create connection";
            return false;
        }

        standbyTimer.start();

        return true;

    }

public slots:

    void closeConnection() {

        qDebug() << "  - SocketSyncExecutor: disconnessione automatica";
        standbyTimer.stop();
        socket.close();

    }

};

}

}

#endif // SOCKET_SYNC_EXECUTOR_HPP
