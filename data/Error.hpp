#ifndef ERROR_HPP
#define ERROR_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

// TODO NIC 26/10/2018: capire come gestire gli errori
enum class ErrorType {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Error {
public:
    using Ptr = Error*;
    using ConstPtr = Error*;

private:
    // TODO NIC 25/10/2018: vedere come creare gli errori
    int deviceKey;
    int errorId;
    QString errorDescription;
    ErrorType errorType;

public:
    Error(int deviceKey, int errorId, QString errorDescription, ErrorType errorType) :
        deviceKey(deviceKey), errorId(errorId),
        errorDescription(errorDescription), errorType(errorType) { }

    int getErrorId() const { return errorId; }
    QString getErrorDescription() const { return errorDescription; }
    ErrorType getErrorType() const { return errorType; }
    int getDeviceKey() const { return deviceKey; }

};


}

#endif // ERROR_HPP
