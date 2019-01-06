#ifndef ABSTRACT_RECEIVER_HPP
#define ABSTRACT_RECEIVER_HPP

#include <QByteArray>
#include <QObject>

namespace ipg {

/**
 * @class AbstractReceiver
 * @brief The AbstractReceiver class
 *
 * Interfaccia per la ricezione di dati.
 * Contiene i metodi readBytes(QByteArray& bytes) che si occuperanno di leggere tutti i dati ricevuti o solo una
 * porzione readBytes(QByteArray& bytes, int count);
 * Nel caso di lettura parziale, si puo verificare se ci sono altri dati da leggere usando il metodo hasData()
 *
 * @note Alla ricezione di un pacchetto di dati, e' obbligatorio notificare la ricezione del pacchetto usando il segnale
 * dataArrives()
 *
 */
class AbstractReceiver : public QObject {
    Q_OBJECT

public:
    typedef AbstractReceiver* Ptr;
    typedef const AbstractReceiver* ConstPtr;

public:

    AbstractReceiver(QObject* parent = nullptr) : QObject(parent) { }

    virtual bool hasData() = 0;

    virtual int readBytes(QByteArray& bytes) = 0;

    virtual int readBytes(QByteArray& bytes, int count) = 0;

signals:
    /**
     * @brief dataArrives
     * questo segnale va assolutamente legato al segnale di arrivo dati;
     * altrimenti all'arrivo dei dati non succede nulla
     */
    void dataArrives();

};

}

#endif // RECEIVER_HPP
