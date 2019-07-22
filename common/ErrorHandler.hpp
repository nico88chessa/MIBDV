#ifndef ERRORMANAGER_HPP
#define ERRORMANAGER_HPP

#include <type_traits>
#include <utility>

#include <QObject>
#include <QList>

#include <Error.hpp>
#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>


namespace PROGRAM_NAMESPACE {

#define DECL_ERROR_SIGNALER_FRIENDS(TYPE) \
    friend void ErrorManager::subscribeObject<TYPE>(TYPE& object); \
    template <typename> friend class hasErrorSignaler; \
    QScopedPointer<ErrorSignaler> errorSignaler; \
    ErrorSignaler* getErrorSignaler() const { \
        return errorSignaler.data(); }


/*
 * E R R O R   S I G N A L E R
 */

class ErrorSignaler : public QObject {
    Q_OBJECT

public:
    using Ptr = ErrorSignaler*;
    using ConstPtr = ErrorSignaler*;

private:
    QList<Error> errors;
    QList<Error> oldErrors;

public:
    explicit ErrorSignaler(QObject* parent = nullptr);

    virtual void addError(const Error& err);
    virtual void addErrors(const QList<Error>& errors);
    virtual void removeError(const Error& err);
    virtual void removeErrors(const QList<Error>& errors);
    virtual void clearErrors();
    virtual void notifyErrors();
    QList<Error> getErrors() const;

signals:
    void signalErrorListUpdate(QList<Error> newErrors, QList<Error> errorsToRemove);

};



/*
 * E R R O R   M A N A G E R
 */

class ErrorManager : public QObject {
    Q_OBJECT

public:
    using Ptr = ErrorManager*;
    using ConstPtr = ErrorManager*;

private:
    QList<Error> machineErrors;

public:
    explicit ErrorManager(QObject* parent = nullptr);

private slots:
    void errorListHandler(QList<Error> newErrors, QList<Error> errorsToRemove);

public:

    template <typename T>
    void subscribeObject(T& object) {

        traceEnter;

        static_assert (hasErrorSignaler<T>::value, "subscribeObject: oggetto non valido");

        /* NOTE NIC 18/07/2019 - AutoConnection
         * non servirebbe autoConnection come parametro perche' e' di default,
         * tuttavia lo metto per questione di chiarezza; la cosa importante e'
         * che lo slot associato al signal DEVE ESSERE ESEGUITO all'interno del thread in cui l'oggetto
         * dello slot vive (altrimenti ci possono essere problemi di concorrenza)
         */
        connect(object.getErrorSignaler(), &ErrorSignaler::signalErrorListUpdate,
                this, &ErrorManager::errorListHandler, Qt::AutoConnection);

        traceExit;

    }

signals:
    void notifyMaxErrorType(const ErrorType& errType);
    void errorListUpdated(const QList<Error> errorList);
    void hasFatals(const QList<Error> fatals);
    void hasErrors(const QList<Error> errors);
    void hasWarnings(const QList<Error> warnings);

};


inline static void initializeErrorSignaler(QScopedPointer<ErrorSignaler>& es, QObject* parent = Q_NULLPTR) {
    es.reset(new ErrorSignaler(parent));
}


template<typename T>
class hasErrorSignaler {
    using yes = char[1];
    using no = char[2];
    template <typename C> static yes& test(decltype(std::declval<C>().getErrorSignaler()));
    template <typename C> static no&  test(...);
  public:
    static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};

}

#endif // ERRORMANAGER_HPP
