#ifndef GALILCNCONTROLLERUTILS_HPP
#define GALILCNCONTROLLERUTILS_HPP

#include <gclibo.h>
#include <gclib_errors.h>

#include <configure.h>
#include <Logger.hpp>

#include <QString>

namespace PROGRAM_NAMESPACE {

class GalilCNControllerUtils {

public:
    GalilCNControllerUtils();

    static QString getErrorDescription(int error);

    static QString getTCDescription(int rcCode);
};

}

#endif // GALILCNCONTROLLERUTILS_HPP
