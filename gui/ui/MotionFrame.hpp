#ifndef MOTIONFRAME_HPP
#define MOTIONFRAME_HPP

#include <MotionBean.hpp>
#include <QFrame>

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

private:
    void setupSignalsAndSlots();

private slots:
    void initializeUI();

public:
    explicit MotionFrame(QWidget *parent = nullptr);
    ~MotionFrame();

    void init();

public slots:
    void updateUI(const PROGRAM_NAMESPACE::MotionBean& bean);

};

#endif // MOTIONFRAME_HPP
