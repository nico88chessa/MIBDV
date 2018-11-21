#ifndef MOTIONFRAMELOGIC_HPP
#define MOTIONFRAMELOGIC_HPP

#include <QObject>

class MotionFrame;

class MotionFrameLogic : public QObject {
    Q_OBJECT

    friend class MotionFrame;

public:
    using Ptr = MotionFrameLogic*;
    using ConstPtr = MotionFrameLogic*;

private:
    MotionFrame* qPtr;

public:
    MotionFrameLogic();

    ~MotionFrameLogic();

};

#endif // MOTIONFRAMELOGIC_HPP
