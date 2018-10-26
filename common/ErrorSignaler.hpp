#ifndef ERRORSIGNALER_HPP
#define ERRORSIGNALER_HPP

#include <QObject>
#include <QList>

#include <configure.h>
#include <Error.hpp>

namespace PROGRAM_NAMESPACE {

class ErrorSignaler : public QObject {
    Q_OBJECT

public:
    using Ptr = ErrorSignaler*;
    using ConstPtr = ErrorSignaler*;

protected:
    QList<Error> errors;

public:
    explicit ErrorSignaler(QObject* parent = nullptr);

    virtual void addError(const Error& err) = 0;
    virtual void addErrors(const QList<Error>& errors) = 0;
    virtual void removeError(const Error& err) = 0;

protected:
    virtual void notifyErrors();

signals:
    void signalErrorListUpdate(QList<Error> list);

};

}

#endif // ERRORSIGNALER_HPP
