#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>

#include "GalilCNController.hpp"

#include <Logger.hpp>
#include <Settings.hpp>

#include "configure.h"

int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

    using namespace PROGRAM_NAMESPACE;

    traceInfo() << "Start applicativo" << APPLICATION_NAME;
    traceEnter;

    traceDebug() << "ciaomiao" << "due";

    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);
    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

    traceExit;

    GalilCNController test;
    test.connect("169.254.12.10");

    GDataRecord2103 record;
    test.getRecord(record);

    int inputStatus;
    for (int i=0; i<9; ++i)
        test.getInput(i, inputStatus);

    Settings::instance();

    traceExit;

    return 0;

}
