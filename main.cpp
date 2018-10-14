#include <QApplication>
#include <QDebug>

#include <Logger.hpp>

#include "configure.h"

int main(int argc, char** argv) {

    using namespace mibdv;

    traceEnter;

    traceDebug() << "ciaomiao" << "due";

    traceExit;

    traceDebug() << APPLICATION_NAME;

    return 0;

}
