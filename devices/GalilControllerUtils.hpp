#ifndef GALILCONTROLLERUTILS_HPP
#define GALILCONTROLLERUTILS_HPP

#include <gclibo.h>
#include <gclib_errors.h>

#include <configure.h>
#include <Logger.hpp>

#include <QString>

namespace PROGRAM_NAMESPACE {

class GalilControllerUtils {

public:
    GalilControllerUtils();

    static QString getErrorDescription(int error);

    static QString getTCDescription(int rcCode);
};

}

#endif // GALILCONTROLLERUTILS_HPP
