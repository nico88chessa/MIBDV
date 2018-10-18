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

    static QString getErrorString(int error);
};

}

#endif // GALILCNCONTROLLERUTILS_HPP
