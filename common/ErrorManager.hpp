#ifndef ERRORMANAGER_HPP
#define ERRORMANAGER_HPP

#include <utility>

#include <QObject>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>
#include <ErrorSignaler.hpp>

#define DECL_ERROR_SIGNALER_FRIENDS(TYPE) \
    friend void ErrorManager::subscribeObject<TYPE>(TYPE& object); \
    friend struct ErrorManager::hasErrorSignaler<TYPE>; \
    QScopedPointer<ErrorSignaler> errorSignaler;

namespace PROGRAM_NAMESPACE {

class ErrorManager : public QObject {
    Q_OBJECT

public:
    using Ptr = ErrorManager*;
    using ConstPtr = ErrorManager*;

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

    template <typename T>
    struct hasErrorSignaler {
        static constexpr bool value = std::is_same<QScopedPointer<ErrorSignaler>, decltype(T::errorSignaler)>::value;
    };

};


}

#endif // ERRORMANAGER_HPP
