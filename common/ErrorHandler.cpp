#include "ErrorHandler.hpp"


using namespace PROGRAM_NAMESPACE;


/*
 * E R R O R   S I G N A L E R
 */

ErrorSignaler::ErrorSignaler(QObject* parent) : QObject(parent) { }

void ErrorSignaler::removeError(const Error& err) {

    if (this->errors.contains(err))
        errors.removeAt(errors.indexOf(err));

}

void ErrorSignaler::removeErrors(const QList<Error>& errors) {

    for (auto&& err: errors)
        this->removeError(err);
}

void ErrorSignaler::clearErrors() {

    this->errors.clear();

}

void ErrorSignaler::addErrors(const QList<Error>& errors) {

    for (Error current: errors)
        this->addError(current);

}

void ErrorSignaler::addError(const Error& err) {

    if (!errors.contains(err))
        errors.append(err);

}

void ErrorSignaler::notifyErrors() {

    /* NOTE NIC 09/07/2019 - prima di segnalare gli errori, confronto quelli aggiornati con
     * quelli gia' segnalati; se nella nuova lista non ci sono piu' quelli gia' segnalati,
     * significa che l'errore non c'e' piu'. Allora lo inserisco nella lista degli errori
     * da rimuovere;
     * inoltre rimuovo gli errori che sono gia' presenti ma hanno un tipo di errore diverso;
     * in questo caso aggiorno l'errore perche' e' cambiato il tipo di errore
     */
    QList<Error> errors2remove;
    for (auto&& err: oldErrors)
        if (!errors.contains(err))
            errors2remove.append(err);
        else {
            int index = errors.indexOf(err);
            Error oldError = errors.at(index);
            if (oldError.getErrorType() != err.getErrorType())
                errors2remove.append(err);
        }

    // notifico gli errori sono se quelli nuovi sono diversi da quelli gia' segnalati
    if (errors2remove.isEmpty() && (oldErrors == errors))
        return;

    emit signalErrorListUpdate(errors, errors2remove);

    oldErrors.clear();
    oldErrors.append(errors);

}

QList<Error> ErrorSignaler::getErrors() const { return errors; }



/*
 * E R R O R   M A N A G E R
 */

ErrorManager::ErrorManager(QObject* parent) : QObject(parent), machineErrors() {

}

void ErrorManager::errorListHandler(QList<Error> newErrors, QList<Error> errorsToRemove) {

    // TODO NIC 26/10/2018 - gestire la lista degli errori

    traceEnter;
    QList<Error> fatals;
    QList<Error> errors;
    QList<Error> warnings;
    QList<Error> info;

    for (auto&& err2Remove: errorsToRemove) {
        int index = machineErrors.indexOf(err2Remove);
        if (index > -1)
            machineErrors.removeAt(index);
    }

    for (auto&& error: newErrors) {
        if (!machineErrors.contains(error))
            machineErrors.append(error);

        switch (error.getErrorType()) {
        case ErrorType::WARNING:
            warnings.append(error);
            break;
        case ErrorType::ERROR:
            errors.append(error);
            break;
        case ErrorType::FATAL:
            fatals.append(error);
            break;
        }
    }

    emit errorListUpdated(this->machineErrors);

    if (!fatals.isEmpty()) emit hasFatals(fatals);
    if (!errors.isEmpty()) emit hasErrors(errors);
    if (!warnings.isEmpty()) emit hasWarnings(warnings);

    traceExit;

}
