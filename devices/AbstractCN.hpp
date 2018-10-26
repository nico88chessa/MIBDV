#ifndef ABSTRACTCN_HPP
#define ABSTRACTCN_HPP

#include <configure.h>
#include "AbstractDevice.hpp"

namespace PROGRAM_NAMESPACE {

template <typename S, typename E>
class AbstractCN : public AbstractDevice<S> {
public:
    using Ptr = AbstractCN*;
    using ConstPtr = const AbstractCN*;

public:
    using posType = int;
    using spdType = double;
    using accType = double;
    using anlType = double;

public:
    enum class Axis { X, Y, Z, W };

public:
    virtual E getDigitalInput(int input, int& inputStatus) = 0;
    virtual E getDigitalOutput(int output, int& outputStatus) = 0;
    virtual E getAnalogInput(int analogInput, anlType& analogInputStatus) = 0;
    virtual E getPosition(Axis a, posType& pos) = 0;
    virtual E isAxisInMotion(Axis a, bool& inMotion) = 0;
    virtual E isAxisPositionError(Axis a, bool& isPositionError) = 0;
    virtual E isMotorOff(Axis a, bool& isMotorOff) = 0;
    virtual E isForwardLimit(Axis a, bool& isForwardLimit) = 0;
    virtual E isBackwardLimit(Axis a, bool& isForwardLimit) = 0;
    virtual E isHomeAxis(Axis a, bool& isHome) = 0;
    virtual E setDigitalOutput(int output, bool value) = 0;
    virtual E setSpeeds(Axis a, spdType speed) = 0;
    virtual E setAccelerations(Axis a, accType acc, accType dec) = 0;
    virtual E setMoveParameters(Axis a, spdType speed, accType acc, accType dec) = 0;
    virtual E stopAxis(Axis a) = 0;
    virtual E homingX(spdType speed, accType acc, accType dec) = 0;
    virtual E homingY(spdType speed, accType acc, accType dec) = 0;
    virtual E homingZ(spdType speed, accType acc, accType dec) = 0;
    virtual E homingW(spdType speed, accType acc, accType dec) = 0;
    virtual E home(Axis a, spdType speed, accType acc, accType dec) = 0;
    virtual E startMoveAxis(Axis a) = 0;
    virtual E moveToPosition(Axis a, posType pos, spdType speed, accType acc, accType dec) = 0;
    virtual E setPosition(Axis a, posType pos) = 0;

};

// type traits

template <typename T>
struct isCN {
    static constexpr bool value = false;
    using statusType = void*;
    using errorType = void*;
};

template <typename S, typename E>
struct isCN<AbstractCN<S, E>> {
    static constexpr bool value = true;
    using statusType = S;
    using errorType = E;
};

template <typename S, typename E>
struct isDevice<AbstractCN<S, E> > {
    static constexpr bool value = true;
    using statusType = S;
};

}

#endif // ABSTRACTCN_HPP
