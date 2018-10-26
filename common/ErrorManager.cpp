#include "ErrorManager.hpp"

using namespace PROGRAM_NAMESPACE;

ErrorManager::ErrorManager(QObject* parent) : QObject(parent) {

}

void ErrorManager::errorListHandler(QList<Error> test) {

    // TODO NIC 26/10/2018 - gestire la lista degli errori

    traceEnter;

    traceDebug() << "Test OK";
    traceDebug() << test.size();

    traceExit;

}
