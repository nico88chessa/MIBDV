#ifndef ERRORMANAGER_HPP
#define ERRORMANAGER_HPP

#include <type_traits>
#include <utility>

#include <QObject>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>
#include <ErrorSignaler.hpp>


#define DECL_ERROR_SIGNALER_FRIENDS(TYPE) \
    friend void ErrorManager::subscribeObject<TYPE>(TYPE& object); \
    QScopedPointer<ErrorSignaler> errorSignaler;


namespace PROGRAM_NAMESPACE {

class ErrorManager : public QObject {
    Q_OBJECT

public:
    using Ptr = ErrorManager*;
    using ConstPtr = ErrorManager*;

private:
    QList<Error> machineErrors;

public:
    explicit ErrorManager(QObject* parent = nullptr);

public slots:
    void errorListHandler(QList<Error> test);

public:

    template <typename T>
    void subscribeObject(T& object) {

        traceEnter;

        static_assert (hasErrorSignaler<T>::value, "subscribeObject: oggetto non valido");
        connect(object.errorSignaler.data(), &ErrorSignaler::signalErrorListUpdate, this, &ErrorManager::errorListHandler);

        traceExit;

    }

signals:
    void errorListUpdated(const QList<Error> errors);

};


template <typename T, typename = void>
struct hasErrorSignaler {
    static constexpr bool value = false;
};

template <typename T>
struct hasErrorSignaler<T,
        typename std::enable_if_t<
            std::is_same<QScopedPointer<ErrorSignaler>, decltype(T::errorSignaler)>::value> > {
    static constexpr bool value = true;
};


}

#endif // ERRORMANAGER_HPP
