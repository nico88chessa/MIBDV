#ifndef MOTIONFRAME_HPP
#define MOTIONFRAME_HPP

#include <QFrame>

namespace Ui {
class MotionFrame;
}

class MotionFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MotionFrame(QWidget *parent = nullptr);
    ~MotionFrame();

private:
    Ui::MotionFrame *ui;
};

#endif // MOTIONFRAME_HPP
