#ifndef IPG_LASER_TEMP_ABSTRACT_SYNC_EXECUTOR_HPP
#define IPG_LASER_TEMP_ABSTRACT_SYNC_EXECUTOR_HPP

#include <QByteArray>
#include <Logger.hpp>

#include <laser-ipg-temporary/marshalling/marshaller_interface.hpp>
#include <laser-ipg-temporary/utility/constants.hpp>

namespace ipg {

static const int SYNC_EXECUTOR_MARSHALLING_ERROR = 1;
static const int SYNC_EXECUTOR_UNMARSHALLING_ERROR = 2;
static const int SYNC_EXECUTOR_SEND_ERROR = 3;

/**
 * @brief The AbstractSyncExecutor class
 *
 * Classe utilizzata per scambio di informazioni sincrono;
 * una volta spedito il messaggio, la funzione aspetta la risposta;
 * se non la riceve entro tot tempo, ritorna un codice d'errore
 */
class AbstractSyncExecutor : public QObject {
    Q_OBJECT

public:
    typedef AbstractSyncExecutor* Ptr;
    typedef const AbstractSyncExecutor* ConstPtr;

protected:

    /**
     * @brief funzione utilizzata per spedire/ricevere il bytestream
     * @param bytesIn dati da inviare
     * @param bytesOut dati da ricevere
     * @return 0 se OK, > 0 altrimenti
     */
    virtual int syncCaller(const QByteArray& bytesIn, QByteArray& bytesOut) = 0;

public:

    AbstractSyncExecutor(QObject* parent=0) : QObject(parent) { }

    /**
     * @brief execute prepara l'input come bytestream e valorizza l'output da bytestream;
     * la funzione di spedire il bytestream e' demandata al metodo syncCaller
     * @param input: bean di input da passare
     * @param output: bean di output da ricevere
     * @return 0 se OK, > 0 altrimenti
     */
    template <typename I, typename O, typename M>
    int execute(const I& input, O& output) {

        using namespace PROGRAM_NAMESPACE;
        MarshallerInterface::Ptr marshaller = new M();

        QByteArray bytesIn;

        if (!marshaller->marshall(&input, bytesIn)) {
            delete marshaller;
            traceWarn() << "Errore SyncExecutor nel metodo marshall.";
            return SYNC_EXECUTOR_MARSHALLING_ERROR;
        }

        QByteArray bytesOut;
        int errorCode = syncCaller(bytesIn, bytesOut);
        if (errorCode > 0) {
            delete marshaller;
            traceWarn() << "Errore SyncExecutor nel metodo syncCaller con codice errore:" << errorCode;
            return SYNC_EXECUTOR_SEND_ERROR;
        }

        if (!marshaller->unmarshall(bytesOut, &output)) {
            delete marshaller;
            traceWarn() << "Errore SyncExecutor nel metodo unmarshall.";
            return SYNC_EXECUTOR_UNMARSHALLING_ERROR;
        }

        delete marshaller;

        return 0;

    }

};

}

#endif // ABSTRACT_SYNC_EXECUTOR_HPP
