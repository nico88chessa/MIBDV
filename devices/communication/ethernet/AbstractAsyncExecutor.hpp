#ifndef ABSTRACT_ASYNC_EXECUTOR_HPP
#define ABSTRACT_ASYNC_EXECUTOR_HPP

#include <QByteArray>
#include <Logger.hpp>

#include "MarshallerInterface.hpp"
#include "AbstractSender.hpp"
#include "AbstractReceiver.hpp"

namespace communication {

namespace ethernet {

static const int ASYNC_EXECUTOR_MARSHALLING_ERROR = 1;
static const int ASYNC_EXECUTOR_SEND_ERROR = 6;

/**
 * @brief The AbstractAsyncExecutor class
 *
 * La classe offre un'interfaccia per la ricezione e invio di messaggi asincrono.
 * Per l'invio dei dati viene utilizzato il metodo send(const I& input);
 * Per la ricezione invece, non c'e' alcuna implementazione in quanto l'interpretazione
 * del dato ricevuto dipende dal formato del dato stesso.
 *
 * @note questa classe richiede l'uso di un Sender per l'invio dei dati e di un
 * Receiver per la ricezione.
 *
 */

namespace pn = PROGRAM_NAMESPACE;

class AbstractAsyncExecutor : public QObject {
    Q_OBJECT

public:
    typedef AbstractAsyncExecutor* Ptr;
    typedef const AbstractAsyncExecutor* ConstPtr;

protected:
    AbstractSender::Ptr sender;
    AbstractReceiver::Ptr receiver;

public:

    AbstractAsyncExecutor(AbstractSender::Ptr _s, AbstractReceiver::Ptr _r, QObject* parent = nullptr) :
        QObject(parent), sender(_s), receiver(_r) {

        connect(_r, SIGNAL(dataArrives()), this, SLOT(receive()));

    }

    /**
     * @brief send metodo utilizzato per inviare i dati
     * @param input dato da inviare
     * @return codice errore: 0 OK, > 0 altrimenti
     */
    template <typename I, typename M>
    int send(const I& input) {

        MarshallerInterface::Ptr marshaller = new M();

        QByteArray bytes;

        if (!marshaller->marshall(&input, bytes)) {
            delete marshaller;
            pn::traceWarn() << "  - Abstract async executor: errore nel metodo marshall.";
            return ASYNC_EXECUTOR_MARSHALLING_ERROR;
        }

//        qDebug() << "  - Abstract async executor: spedisco:" << bytes;
        int errorCode = sender->sendData(bytes);
        if (errorCode > 0) {
            delete marshaller;
            pn::traceWarn() << "  - Abstract async executor: errore nel metodo sendData con codice errore:" << errorCode;
            return ASYNC_EXECUTOR_SEND_ERROR;
        }

        delete marshaller;

        return 0;

    }

public slots:

    /**
     * @brief receive
     *
     * metodo che si occupera' della ricezione dei dati. L'implementazione e'
     * demandata in quanto la gestione del messaggio ricevuto dipende dal tipo
     * di protocollo utilizzato;
     * @return un intero = 0 se la ricezione e' andata a buon fine, > 0 altrimenti
     */
    virtual int receive() = 0;

};

}

}

#endif // ABSTRACT_ASYNC_EXECUTOR_HPP
