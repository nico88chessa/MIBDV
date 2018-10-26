#ifndef GALILCNCONTROLLER_HPP
#define GALILCNCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <Logger.hpp>
#include <devices/GalilControllerUtils.hpp>
#include <devices/AbstractCN.hpp>

namespace PROGRAM_NAMESPACE {

class GalilCNController : public AbstractCN<GDataRecord2103, int> {
public:
    using Ptr = GalilCNController *;
    using ConstPtr = const GalilCNController *;

    static constexpr DeviceKey deviceKey = DeviceKey::GALIL_CN;

private:
    QScopedPointer<GCon> handler;
    bool isInitialized;
    bool connected;
    int numDigitalInput;
    int numDigitalOutput;
    int numAnalogInput;

    static constexpr const int NUM_IO_PER_BANK = 8;

    static constexpr char letterFromAxis(Axis v) {
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

    void setupController(int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    bool connect(const QString& ip);
    int getRecord(GDataRecord2103& record) const;
    virtual int getDigitalInput(int input, int& inputStatus);
    virtual int getDigitalOutput(int output, int& outputStatus);
    virtual int getAnalogInput(int analogInput, anlType& analogInputStatus);
    virtual int getPosition(Axis a, posType& pos);
    virtual int isAxisInMotion(Axis a, bool& inMotion);
    virtual int isAxisPositionError(Axis a, bool& isPositionError);
    virtual int isMotorOff(Axis a, bool& isMotorOff);
    virtual int isForwardLimit(Axis a, bool& isForwardLimit);
    virtual int isBackwardLimit(Axis a, bool& isForwardLimit);
    virtual int isHomeAxis(Axis a, bool& isHome);
    bool checkAbort(bool& isAbort);
    virtual int setDigitalOutput(int output, bool value);
    virtual int setSpeeds(Axis a, spdType speed);
    virtual int setAccelerations(Axis a, accType acc, accType dec);
    virtual int setMoveParameters(Axis a, spdType speed, accType acc, accType dec);
    virtual int stopAxis(Axis a);
    virtual int homingX(spdType speed, accType acc, accType dec);
    virtual int homingY(spdType speed, accType acc, accType dec);
    virtual int homingZ(spdType speed, accType acc, accType dec);
    virtual int homingW(spdType speed, accType acc, accType dec);
    virtual int home(Axis a, spdType speed, accType acc, accType dec);
    virtual int startMoveAxis(Axis a);
    virtual int moveToPosition(Axis a, posType pos, spdType speed, accType acc, accType dec);
    virtual int setPosition(Axis a, posType pos);
    int getTCCode(int& tcCode) const;
    bool isConnected() const;
    virtual GDataRecord2103 getStatus() const;

private:
    inline GCon handle() const { return *this->handler.data(); }
    inline void writeError(int errorCode) const;
    inline void writeErrorIfExists(int errorCode) const;
    int getInputs(int bank, int& bankStatus);
    int tellSwitches(Axis a, int& value);


};

// type traits

template <>
struct isCN<GalilCNController> {
    static constexpr bool value = true;
    using statusType = GDataRecord2103;
    using errorType = int;
};

template <>
struct isDevice<GalilCNController> {
    static constexpr bool value = true;
    using statusType = GDataRecord2103;
};

}

#endif // GALILCNCONTROLLER_HPP
