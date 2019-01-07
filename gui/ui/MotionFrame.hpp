#ifndef MOTIONFRAME_HPP
#define MOTIONFRAME_HPP

#include <QSharedPointer>
#include <QFrame>

#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <MotionBean.hpp>
#include <IOInspector.hpp>

class MotionFrameLogic;

namespace Ui {
class MotionFrame;
}

class MotionFrame : public QFrame {
    Q_OBJECT

    friend class MotionFrameLogic;

public:
    using Ptr = MotionFrame *;
    using ConstPtr = const MotionFrame *;

private:
    static constexpr int MOTION_FRAME_CYCLE_ERR = PROGRAM_NAMESPACE::PROGRAM_ERR_START_CODE + 1;
    static constexpr int MOTION_FRAME_DSB_MIN_VALUE = -100000;
    static constexpr int MOTION_FRAME_DSB_MAX_VALUE = 100000;

    Ui::MotionFrame *ui;
    MotionFrameLogic* dPtr;
    PROGRAM_NAMESPACE::MotionBean motionBean;
    PROGRAM_NAMESPACE::DigitalInputStatus digitalInputStatus;
    bool isHomingAxes;

    static QString decodeError(int error) {

        QString errDescr;
        switch (error) {
            case MOTION_FRAME_CYCLE_ERR: errDescr = tr("Abilitare CYCLE per iniziare la movimenazione."); break;
            default: ;
        }
        return errDescr;

    }

public:
    explicit MotionFrame(QWidget *parent = nullptr);
    ~MotionFrame();

    void setupDevices(const QSharedPointer<PROGRAM_NAMESPACE::MotionManager>& motionManager,
            const QSharedPointer<PROGRAM_NAMESPACE::IOManager>& ioManager);

private:
    void setupUi();
    void setupSignalsAndSlots();

public slots:
    void updateMotionBean(const PROGRAM_NAMESPACE::MotionBean& b);
    void updateDigitalInputStatus(const PROGRAM_NAMESPACE::DigitalInputStatus& i);

private slots:
    void updateUI();

};

#endif // MOTIONFRAME_HPP
