#ifndef GALILCNCONTROLLER_HPP
#define GALILCNCONTROLLER_HPP

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <data/GalilCNStatusBean.hpp>
#include <devices/AbstractCN.hpp>

namespace PROGRAM_NAMESPACE {

/**
 * @brief The GalilCNController class
 * Classe utilizzata per controllare il controller Galil
 * Nota importante:
 * utilizzando il server GCaps, il server chiude la connessione con il client automaticamente
 * dopo un periodo di default di 10 min;
 * per evitare questo, e' necessario effettuare almeno una chiamata al server
 * (vedere documentazione gcaps parametro G_UTIL_GCAPS_KEEPALIVE)
 */

class GalilCNController : public AbstractCN<GalilCNStatusBean, int> {
public:
    using Ptr = GalilCNController *;
    using ConstPtr = const GalilCNController *;

    static constexpr DeviceKey deviceKey = DeviceKey::GALIL_CN;

private:
    QScopedPointer<GCon> handler;
    bool isInitialized;
    bool connectionStatus;
    int numDigitalInput;
    int numDigitalOutput;
    int numAnalogInput;
    bool customHomeAxisX;
    QString handleCode;
    QString ipAddress;
    int commandTimeoutMs;

    static constexpr const int NUM_IO_PER_BANK = 8;

    static char letterFromAxis(Axis v) {
        switch (v) {
        case Axis::X: return 'A';
        case Axis::Y: return 'B';
        case Axis::Z: return 'C';
        case Axis::W: return 'D';
        }
        return '0';
    }

    enum class SwitchBitMask : int {
        POSITION_LATCH_HAS_OCCURRED,
        HOME_SWITCH_STATUS,
        REVERSE_LIMIT_SWITCH_INACTIVE,
        FORWARD_LIMIT_SWITCH_INACTIVE,
        RESERVED,
        MOTOR_OFF,
        POSITION_ERROR_EXCEEDS_ERROR_LIMIT,
        AXIS_IN_MOTION
    };

public:
    GalilCNController();
    virtual ~GalilCNController();

    void setupController(const QString& ipAddress,
                         int commandTimeoutMs,
                         int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput,
                         bool customHomeAxisX);
    virtual bool connect() override;
    int getRecord(GalilCNStatusBean& record);
    virtual int getDigitalInput(int input, int& inputStatus);
    virtual int getDigitalOutput(int output, int& outputStatus);
    virtual int getAnalogInput(int analogInput, anlType& analogInputStatus);
    virtual int getPosition(Axis a, posCNType& pos);
    virtual int isAxisInMotion(Axis a, bool& inMotion);
    virtual int isAxisPositionError(Axis a, bool& isPositionError);
    virtual int isMotorOff(Axis a, bool& isMotorOff);
    virtual int isForwardLimit(Axis a, bool& isForwardLimit);
    virtual int isBackwardLimit(Axis a, bool& isBackwardLimit);
    virtual int isHomeAxis(Axis a, bool& isHome);
    bool checkAbort(bool& isAbort);
    virtual int setDigitalOutput(int output, bool value);
    virtual int setSpeeds(Axis a, spdCNType speed);
    virtual int setAccelerations(Axis a, accCNType acc, accCNType dec);
    virtual int setMoveParameters(Axis a, spdCNType speed, accCNType acc, accCNType dec);
    virtual int stopAxis(Axis a);
    virtual int homingX(spdCNType speed, accCNType acc, accCNType dec);
    virtual int homingY(spdCNType speed, accCNType acc, accCNType dec);
    virtual int homingZ(spdCNType speed, accCNType acc, accCNType dec);
    virtual int homingW(spdCNType speed, accCNType acc, accCNType dec);
    virtual int home(Axis a, spdCNType speed, accCNType acc, accCNType dec);
    virtual int startMoveAxis(Axis a);
    virtual int moveToPosition(Axis a, posCNType pos, spdCNType speed, accCNType acc, accCNType dec);
    virtual int setPosition(Axis a, posCNType pos);
    int getTCCode(int& tcCode);
    virtual bool isConnected() override;
    virtual GalilCNStatusBean getStatus();
    virtual bool isError(int errorCode) { return errorCode != G_NO_ERROR; }
    virtual QString decodeError(const int& errorCode);
    int getKeepAliveTimeMs(unsigned int* timeMs, unsigned int* newValue = nullptr); // 10 min valore default

private:
    inline bool getConnectionStatus() const { return connectionStatus; }
    inline void setConnectionStatus(bool value) { connectionStatus = value; }
    int disconnect();
    inline GCon handle() const { return *this->handler.data(); }
    inline void writeError(int errorCode);
    inline void writeErrorIfExists(int errorCode);
    int getInputs(int bank, int& bankStatus);
    int tellSwitches(Axis a, int& value);

};

// type traits

template <>
struct isCN<GalilCNController> {
    static constexpr bool value = true;
    using statusType = GalilCNStatusBean;
    using errorType = int;
};

template <>
struct isDevice<GalilCNController> {
    static constexpr bool value = true;
    using statusType = GalilCNStatusBean;
};

}

#endif // GALILCNCONTROLLER_HPP
