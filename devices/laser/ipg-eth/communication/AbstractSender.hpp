#ifndef ABSTRACT_SENDER_HPP
#define ABSTRACT_SENDER_HPP

#include <QByteArray>
#include <QObject>

namespace ipg {

/**
 * @brief The AbstractSender class
 *
 * Interfaccia per l'invio di dati.
 * Contiene l'unico metodo sendData(const QByteArray& data), che si dovra' occupare di inviare i dati utilizzando un QByteArray
 */
class AbstractSender : public QObject {
    Q_OBJECT

public:
    typedef AbstractSender* Ptr;
    typedef const AbstractSender* ConstPtr;

public:

    AbstractSender(QObject* parent=0) : QObject(parent) { }

public slots:

    /**
     * @brief sendData
     * Metodo da utilizzare per inviare i dati
     * @param data dati da inviare
     * @return un intero = 0 se l'invio e' andato a buon fine, > 0 altrimenti; la codifica dell'errore
     * e' demandata all'implementazione dell'interfaccia
     */
    virtual int sendData(const QByteArray& data) = 0;

};

}

#endif // SENDER_HPP
