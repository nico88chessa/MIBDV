#ifndef ERROR_HPP
#define ERROR_HPP

#include <QString>

#include <configure.h>
#include <Constants.hpp>

#include <Types.hpp>


namespace PROGRAM_NAMESPACE {

static constexpr unsigned int BIT_ID_MASK = 16;
static constexpr unsigned int BIT_INSTANCE_ID_MASK = 8;

class Error {
public:
    using Ptr = Error*;
    using ConstPtr = Error*;

private:
    DeviceKey deviceKey;
    int deviceInstance;
    ErrorID errorId;
    QString errorDescription;
    ErrorType errorType;

public:
    Error() { }
    Error(DeviceKey deviceKey, ErrorID id, QString errorDescription, ErrorType errorType, int deviceInstance = 1) :
        deviceKey(deviceKey), errorId(id),
        errorDescription(errorDescription), errorType(errorType), deviceInstance(deviceInstance) { }

    inline DeviceKey getDeviceKey() const { return deviceKey; }
    inline int getDeviceInstance() const { return deviceInstance; }
    inline ErrorID getErrorId() const { return errorId; }
    inline QString getErrorDescription() const { return errorDescription; }
    inline ErrorType getErrorType() const { return errorType; }

    inline int getErrorKey() const {
        return (static_cast<int>(this->getDeviceKey()) << BIT_ID_MASK) + \
                (this->getDeviceInstance() << BIT_INSTANCE_ID_MASK) + \
                getErrorId();
    }

    friend inline bool operator==(const Error& lhs, const Error& rhs);

};

/* NOTE NIC 15/07/2019 - gestione errori
 * un errore e' uguale ad un altro nel caso in cui proviene dallo stesso dispositivo e ha lo stesso codice.
 * Importante sottolineare che, in fase visualizzazione errori nella pagina AlertFrame, questa uguaglianza
 * non vale piu' perche' bisogna differenziare anche nel caso in cui il tipo di errori sia cambiato
 */
bool operator==(const Error& lhs, const Error& rhs) {
    return lhs.getErrorKey() == rhs.getErrorKey();
//    return (lhs.errorId == rhs.errorId) &&
//           (lhs.deviceKey == rhs.deviceKey);

}

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::Error)

#endif // ERROR_HPP
