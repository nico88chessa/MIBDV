#ifndef MOTIONFRAMELOGIC_HPP
#define MOTIONFRAMELOGIC_HPP

#include <QObject>
#include <QSharedPointer>

#include <MotionManager.hpp>

class MotionFrame;

class MotionFrameLogic : public QObject {
    Q_OBJECT

    friend class MotionFrame;

public:
    using Ptr = MotionFrameLogic*;
    using ConstPtr = MotionFrameLogic*;

private:
    MotionFrame* qPtr;

    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;

public:
    MotionFrameLogic();
    ~MotionFrameLogic();

    void setupLogic(const QSharedPointer<mibdv::MotionManager>& motionManager);

private slots:
    void moveAxisX();

};

#endif // MOTIONFRAMELOGIC_HPP
