#ifndef ERROR_HPP
#define ERROR_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

static constexpr unsigned int BIT_ID_MASK = 7;

// TODO NIC 26/10/2018 - capire come gestire gli errori
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
    // TODO NIC 25/10/2018 - vedere come creare gli errori
    int deviceKey;
    int errorId;
    QString errorDescription;
    ErrorType errorType;

public:
    Error(int deviceKey, int errorId, QString errorDescription, ErrorType errorType) :
        deviceKey(deviceKey), errorId(errorId),
        errorDescription(errorDescription), errorType(errorType) { }

    inline int getErrorId() const { return errorId; }
    inline QString getErrorDescription() const { return errorDescription; }
    inline ErrorType getErrorType() const { return errorType; }
    inline int getDeviceKey() const { return deviceKey; }
    inline int getErrorKey() const { return (this->getDeviceKey() << BIT_ID_MASK) + getErrorId(); }

    friend inline bool operator==(const Error& lhs, const Error& rhs);

};

bool operator==(const Error& lhs, const Error& rhs) {
    return (lhs.errorId == rhs.errorId) &&
           (lhs.deviceKey == rhs.deviceKey);

}

}

#endif // ERROR_HPP
