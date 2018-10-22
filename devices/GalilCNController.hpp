#ifndef GALILCNCONTROLLER_HPP
#define GALILCNCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <utils/Logger.hpp>
#include <devices/GalilCNControllerUtils.hpp>

namespace PROGRAM_NAMESPACE {

class GalilCNController {
    using Ptr = GalilCNController *;
    using ConstPtr = const GalilCNController *;

    using posType = int;
    using spdType = double;
    using accType = double;

public:
    enum class Axis { X, Y, Z, W };

private:
    QScopedPointer<GCon> handler;
    bool isInitialized;
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

private:
    inline GCon handle() { return *this->handler.data(); }
    inline void writeError(int errorCode);
    inline void writeErrorIfExists(int errorCode);
    bool getInputs(int bank, int& bankStatus);
    bool tellSwitches(Axis a, int& value);

public:
    GalilCNController();
    ~GalilCNController();

    void setupController(int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    bool connect(const QString& ip);
    bool getRecord(GDataRecord2103& record);
    bool getDigitalInput(int input, int& inputStatus);
    bool getDigitalOutput(int output, int& outputStatus);
    bool getAnalogInput(int analogInput, int& analogInputStatus);
    bool getPosition(Axis a, posType& pos);
    bool isAxisInMotion(Axis a, bool& inMotion);
    bool isAxisPositionError(Axis a, bool& isPositionError);
    bool isMotorOff(Axis a, bool& isMotorOff);
    bool isForwardLimit(Axis a, bool& isForwardLimit);
    bool isBackwardLimit(Axis a, bool& isForwardLimit);
    bool isHomeAxis(Axis a, bool& isHome);
    bool checkAbort(bool& isAbort);
    bool setDigitalOutput(int output, bool value);
    bool setSpeeds(Axis a, spdType speed);
    bool setAccelerations(Axis a, accType acc, accType dec);
    bool setMoveParameters(Axis a, spdType speed, accType acc, accType dec);
    bool stopAxis(Axis a);
    bool homingX(spdType speed, accType acc, accType dec);
    bool homingY(spdType speed, accType acc, accType dec);
    bool homingZ(spdType speed, accType acc, accType dec);
    bool homingW(spdType speed, accType acc, accType dec);
    bool home(Axis a, spdType speed, accType acc, accType dec);
    bool startMoveAxis(Axis a);
    bool moveToPosition(Axis a, posType pos, spdType speed, accType acc, accType dec);
    bool setPosition(Axis a, posType pos);

};

}

#endif // GALILCNCONTROLLER_HPP
