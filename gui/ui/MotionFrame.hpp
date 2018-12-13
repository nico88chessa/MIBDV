#ifndef MOTIONFRAME_HPP
#define MOTIONFRAME_HPP

#include <QSharedPointer>
#include <QFrame>

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

    static QString decodeError(int error) {

        QString errDescr;
        switch (error) {
            case MOTION_FRAME_CYCLE_ERR: errDescr = tr("Abilitare CYCLE per iniziare la movimenazione."); break;
            default: ;
        }
        return errDescr;

    }

    Ui::MotionFrame *ui;
    MotionFrameLogic* dPtr;
    PROGRAM_NAMESPACE::MotionBean motionBean;
    PROGRAM_NAMESPACE::IOInspector::DigitalInputStatus digitalInputStatus;

public:
    explicit MotionFrame(QWidget *parent = nullptr);
    ~MotionFrame();

    void setupDevices(const QSharedPointer<PROGRAM_NAMESPACE::MotionManager>& motionManager);

private:
    void setupUi();
    void setupSignalsAndSlots();

public slots:
    void updateMotionBean(const PROGRAM_NAMESPACE::MotionBean& b);
    void updateDigitalInputStatus(const PROGRAM_NAMESPACE::IOInspector::DigitalInputStatus& i);
    void updateUI();

private slots:

};

#endif // MOTIONFRAME_HPP
