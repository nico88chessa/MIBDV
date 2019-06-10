#ifndef TESTFRAME_HPP
#define TESTFRAME_HPP

#include <QButtonGroup>
#include <QFrame>
#include <QTabWidget>

#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <MotionBean.hpp>
#include <DigitalInputValue.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>


class TestFrameLogic;

enum class PointShapeEnum : int {
    UNDEFINED = -1,
    POINT,
    CIRCLE_POINTS,
    CIRCLE_VECTOR
};

namespace Ui {
class TestFrame;
}

class TestFrame : public QFrame {
    Q_OBJECT

    friend class TestFrameLogic;

public:
    using Ptr = TestFrame *;
    using ConstPtr = const TestFrame *;

public:
    static constexpr int TEST_FRAME_PULSE_ENERGY_DFLT = 1;

private:
    static constexpr int TEST_FRAME_POINTS_PER_TILE_MIN = 1;
    static constexpr int TEST_FRAME_POINTS_PER_TILE_MAX = 10000;
    static constexpr int TEST_FRAME_TILE_SIZE_MIN = 1;
    static constexpr int TEST_FRAME_TILE_SIZE_MAX = 200;
    static constexpr int TEST_FRAME_MIN_FREQUENCY = 0;
    static constexpr int TEST_FRAME_MAX_FREQUENCY = 3000;
    static constexpr int TEST_FRAME_WAIT_TIME_MS_MIN = 50;
    static constexpr int TEST_FRAME_WAIT_TIME_MS_MAX = 10000;
    static constexpr int TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MIN = 1000;
    static constexpr int TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MAX = 10000;
    static constexpr int TEST_FRAME_OFFSET_X_MIN = 0;
    static constexpr int TEST_FRAME_OFFSET_X_MAX = 1200;
    static constexpr int TEST_FRAME_OFFSET_Y_MIN = 0;
    static constexpr int TEST_FRAME_OFFSET_Y_MAX = 1200;
    static constexpr double TEST_FRAME_SCALE_X_DFLT = 100;
    static constexpr double TEST_FRAME_SCALE_X_MIN = 80;
    static constexpr double TEST_FRAME_SCALE_X_MAX = 120;
    static constexpr double TEST_FRAME_SCALE_Y_DFLT = 100;
    static constexpr double TEST_FRAME_SCALE_Y_MIN = 80;
    static constexpr double TEST_FRAME_SCALE_Y_MAX = 120;
    static constexpr int TEST_FRAME_POINTS_DISTANCE_UM_MIN = 100;
    static constexpr int TEST_FRAME_POINTS_DISTANCE_UM_MAX = 100000;
    static constexpr int TEST_FRAME_ANGLE_MRAD_MIN = -200;
    static constexpr int TEST_FRAME_ANGLE_MRAD_MAX = 200;
    static constexpr double TEST_FRAME_DSB_STEP = 0.1;
    static constexpr double TEST_FRAME_DSB_SCALE_STEP = 0.01;
    static constexpr double TEST_FRAME_ANGLE_STEP = 0.001;

    static constexpr int TEST_FRAME_PULSES_MIN = 1;
    static constexpr int TEST_FRAME_PULSES_MAX = 20000;
    static constexpr int TEST_FRAME_CIRCLE_RADIUS_MIN = 50;
    static constexpr int TEST_FRAME_CIRCLE_RADIUS_MAX = 100000; // TODO NIC 13/05/2019 - rimettere 2000
    static constexpr int TEST_FRAME_CIRCLE_RADIUS_STEP = 1;
    static constexpr int TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_MIN = 1;
    static constexpr int TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_MAX = 2000;
    static constexpr int TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_STEP = 1;
    static constexpr int TEST_FRAME_CIRCLE_NUM_SIDES_MIN = 3;
    static constexpr int TEST_FRAME_CIRCLE_NUM_SIDES_MAX = 20; // TODO NIC 13/05/2019 - rimettere 10
    static constexpr int TEST_FRAME_CIRCLE_NUM_SIDES_STEP = 1;
    static constexpr int TEST_FRAME_CIRCLE_POINTS_PITCH_MIN = 1;
    static constexpr int TEST_FRAME_CIRCLE_POINTS_PITCH_MAX = TEST_FRAME_CIRCLE_RADIUS_MAX/20; // parametro da verificare
    static constexpr int TEST_FRAME_CIRCLE_POINTS_PITCH_STEP = 1;


    static constexpr int TEST_FRAME_LASER_MIN_POWER = 0;
    static constexpr int TEST_FRAME_LASER_MAX_POWER = 100;
    static constexpr int TEST_FRAME_LASER_POWER_STEP = 1;

    static constexpr int TEST_FRAME_LASER_TAB_INDEX = 2;

    Ui::TestFrame *ui;
    QButtonGroup* pointShapeGroup;
    TestFrameLogic* dPtr;
    PROGRAM_NAMESPACE::MotionBean motionBean;
    PROGRAM_NAMESPACE::DigitalInputStatus digitalInputStatus;
    bool laserParametersChanged;

public:
    explicit TestFrame(QWidget *parent = nullptr);
    ~TestFrame();

public slots:
    void updateMotionBean(const PROGRAM_NAMESPACE::MotionBean& b);
    void updateDigitalInputStatus(const PROGRAM_NAMESPACE::DigitalInputStatus& i);
    void setFilePath(const QString& filePath);
    void laserIpgYLPNConfigurationReady();
    void showPopup(const QString& err, const QString& descr);

private:
    void setupUi();
    void setupSignalsAndSlots();
    void updateTabLaserLabel(bool setAsterisk);

    bool eventFilter(QObject *object, QEvent *event);

};

#endif // TESTFRAME_HPP
