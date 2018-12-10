#ifndef MOTIONFRAME_HPP
#define MOTIONFRAME_HPP

#include <QSharedPointer>
#include <QFrame>

#include <MotionManager.hpp>
#include <MotionBean.hpp>

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
    Ui::MotionFrame *ui;
    MotionFrameLogic* dPtr;
    PROGRAM_NAMESPACE::MotionBean bean;

public:
    explicit MotionFrame(QWidget *parent = nullptr);
    ~MotionFrame();

    void setupLogic(const QSharedPointer<PROGRAM_NAMESPACE::MotionManager>& motionManager);

private:
    void setupUi();
    void setupSignalsAndSlots();

public slots:
    void updateUI(const PROGRAM_NAMESPACE::MotionBean& b);

private slots:

};

#endif // MOTIONFRAME_HPP
