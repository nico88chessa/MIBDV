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

private:
    QList<Error> errors;

public:
    explicit ErrorSignaler(QObject* parent = nullptr);

    virtual void addError(const Error& err);
    virtual void addErrors(const QList<Error>& errors);
    virtual void removeError(const Error& err);
    virtual void removeAllErrors();
    virtual void notifyErrors();

protected:
    virtual bool isErrorPresent(const Error& err);

signals:
    void signalErrorListUpdate(QList<Error> list);

};

}

#endif // ERRORSIGNALER_HPP
