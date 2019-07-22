#ifndef MOTIONFRAMELOGIC_HPP
#define MOTIONFRAMELOGIC_HPP

#include <QObject>
#include <QSharedPointer>

#include <MotionManager.hpp>
#include <IOManager.hpp>
#include <IOSignaler.hpp>
#include <devices/MotionAnalizer.hpp>


class MotionFrame;

class MotionFrameLogic : public QObject {
    Q_OBJECT

    friend class MotionFrame;

public:
    using Ptr = MotionFrameLogic*;
    using ConstPtr = MotionFrameLogic*;

private:
    MotionFrame* qPtr;
    QWeakPointer<PROGRAM_NAMESPACE::IMotionAnalizer> motionAnalizer;
    QWeakPointer<PROGRAM_NAMESPACE::IOSignaler> ioSignaler;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;
    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    const int TIMER_CHECK_MOTION_MS = 500;

public:
    MotionFrameLogic();
    ~MotionFrameLogic();

    void initialize();

private:
    bool checkCycle();

private slots:
    void moveX();
    void moveY();
    void moveZ();

    void stopX();
    void stopY();
    void stopZ();

    void homeAxes();
    void stopAxes();

    void airOn();
    void airOff();

};

#endif // MOTIONFRAMELOGIC_HPP
