#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>

#include <Logger.hpp>

#include "configure.h"

int main(int argc, char** argv) {

    using namespace PROGRAM_NAMESPACE;

    qDebug() << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    qDebug() << QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    qDebug() << QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);
    qDebug() << QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    traceEnter;

    traceDebug() << "ciaomiao" << "due";

    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);
    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    traceExit;

//    QSettings prova(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION, APPLICATION_NAME);

    traceDebug() << APPLICATION_NAME;

    return 0;

}
