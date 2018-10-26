#include "ErrorSignaler.hpp"


using namespace PROGRAM_NAMESPACE;

ErrorSignaler::ErrorSignaler(QObject* parent) : QObject(parent) { }

void ErrorSignaler::removeError(const Error& err) {

}

void ErrorSignaler::addErrors(const QList<Error>& errors) {

}

void ErrorSignaler::addError(const Error& err) {

}

void ErrorSignaler::notifyErrors() { emit signalErrorListUpdate(errors); }
