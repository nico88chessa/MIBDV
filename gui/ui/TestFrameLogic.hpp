#ifndef TESTFRAMELOGIC_HPP
#define TESTFRAMELOGIC_HPP

#include <QObject>

#include <MotionManager.hpp>
#include <IOManager.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>

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
    QSharedPointer<ipg::IpgSyncInterface> ipgInterface;

    bool isProcessStopped;
    bool isLaserInitialized;

public:
    TestFrameLogic();
    ~TestFrameLogic();

    void setupMotionManager(const QSharedPointer<mibdv::MotionManager>& motionManager);
    void setupIOManager(const QSharedPointer<mibdv::IOManager>& ioManager);
    void setupLaserIpgYLPN(const QSharedPointer<ipg::IpgSyncInterface>& ipgInterface);

private:
    bool setupLaserOn();
    bool setupLaserOff();

private slots:
    void startProcess();
    void stopProcess();

    void changeGuideLaserState();

};

#endif // TESTFRAMELOGIC_HPP
