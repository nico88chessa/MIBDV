#include "ErrorSignaler.hpp"


using namespace PROGRAM_NAMESPACE;

ErrorSignaler::ErrorSignaler(QObject* parent) : QObject(parent) { }

void ErrorSignaler::removeError(const Error& err) {

    for (Error current: errors)
        if (err == current)
            errors.removeAll(current);

}

void ErrorSignaler::removeAllErrors() {

    this->errors.clear();

}

void ErrorSignaler::addErrors(const QList<Error>& errors) {

    for (Error current: errors)
        this->addError(current);

}

void ErrorSignaler::addError(const Error& err) {

    if (!isErrorPresent(err))
        errors.append(err);

}

void ErrorSignaler::notifyErrors() { emit signalErrorListUpdate(errors); }

bool ErrorSignaler::isErrorPresent(const Error& err) {

    for (Error current: errors)
        if (current == err)
            return true;
    return false;

}
