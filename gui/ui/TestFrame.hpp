#ifndef TESTFRAME_HPP
#define TESTFRAME_HPP

#include <QFrame>
#include <QTabWidget>

#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <MotionBean.hpp>
#include <DigitalInputValue.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>

class TestFrameLogic;

namespace Ui {
class TestFrame;
}

class TestFrame : public QFrame {
    Q_OBJECT

    friend class TestFrameLogic;

public:
    using Ptr = TestFrame *;
    using ConstPtr = const TestFrame *;

private:
    static constexpr int TEST_FRAME_POINTS_PER_TILE_MIN = 0;
    static constexpr int TEST_FRAME_POINTS_PER_TILE_MAX = 10000;
    static constexpr int TEST_FRAME_PULSES_MIN = 0;
    static constexpr int TEST_FRAME_PULSES_MAX = 20000;
    static constexpr int TEST_FRAME_TILE_SIZE_MIN = 0;
    static constexpr int TEST_FRAME_TILE_SIZE_MAX = 200;
    static constexpr int TEST_FRAME_MIN_FREQUENCY = 0;
    static constexpr int TEST_FRAME_MAX_FREQUENCY = 3000;
    static constexpr int TEST_FRAME_WAIT_TIME_MS_MIN = 0;
    static constexpr int TEST_FRAME_WAIT_TIME_MS_MAX = 10000;
    static constexpr int TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MIN = 0;
    static constexpr int TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MAX = 10000;
    static constexpr int TEST_FRAME_OFFSET_X_MIN = 0;
    static constexpr int TEST_FRAME_OFFSET_X_MAX = 1200;
    static constexpr int TEST_FRAME_OFFSET_Y_MIN = 0;
    static constexpr int TEST_FRAME_OFFSET_Y_MAX = 1200;
    static constexpr int TEST_FRAME_POINTS_DISTANCE_UM_MIN = 100;
    static constexpr int TEST_FRAME_POINTS_DISTANCE_UM_MAX = 100000;
    static constexpr int TEST_FRAME_ANGLE_MRAD_MIN = -200;
    static constexpr int TEST_FRAME_ANGLE_MRAD_MAX = 200;
    static constexpr double TEST_FRAME_DSB_STEP = 0.1;
    static constexpr double TEST_FRAME_ANGLE_STEP = 0.001;

    static constexpr int TEST_FRAME_LASER_MIN_POWER = 0;
    static constexpr int TEST_FRAME_LASER_MAX_POWER = 100;
    static constexpr int TEST_FRAME_LASER_POWER_STEP = 1;

    Ui::TestFrame *ui;
    TestFrameLogic* dPtr;
    PROGRAM_NAMESPACE::MotionBean motionBean;
    PROGRAM_NAMESPACE::DigitalInputStatus digitalInputStatus;
    bool laserParametersChanged;

public:
    explicit TestFrame(QWidget *parent = nullptr);
    void setupDevices(const QSharedPointer<PROGRAM_NAMESPACE::MotionManager>& motionManager,
            const QSharedPointer<PROGRAM_NAMESPACE::IOManager>& ioManager,
            const QSharedPointer<ipg::IpgSyncInterface>& ipgInterface);
    ~TestFrame();

public slots:
    void updateMotionBean(const PROGRAM_NAMESPACE::MotionBean& b);
    void updateDigitalInputStatus(const PROGRAM_NAMESPACE::DigitalInputStatus& i);
    void setFilePath(const QString& filePath);
    void laserIpgYLPNConfigurationReady();

private:
    void setupUi();
    void setupSignalsAndSlots();
    void updateTabLaserLabel(bool setAsterisk);

    bool eventFilter(QObject *object, QEvent *event);

};

#endif // TESTFRAME_HPP
