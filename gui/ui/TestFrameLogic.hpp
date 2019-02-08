#ifndef TESTFRAMELOGIC_HPP
#define TESTFRAMELOGIC_HPP

#include <QObject>

#include <MotionManager.hpp>
#include <IOManager.hpp>

class TestFrame;

class TestFrameLogic : public QObject {
    Q_OBJECT

    friend class TestFrame;

public:
    using Ptr = TestFrameLogic *;
    using ConstPtr = const TestFrameLogic *;

private:
    TestFrame* qPtr;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;
    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;

public:
    TestFrameLogic();
    ~TestFrameLogic();

    void setupMotionManager(const QSharedPointer<mibdv::MotionManager>& motionManager);
    void setupIOManager(const QSharedPointer<mibdv::IOManager>& ioManager);

private slots:
    void startProcess();


};

#endif // TESTFRAMELOGIC_HPP
