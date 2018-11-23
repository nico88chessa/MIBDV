#ifndef IOFRAME_HPP
#define IOFRAME_HPP

#include <QFrame>

class IOFrameLogic;

namespace Ui {
class IOFrame;
}

class IOFrame : public QFrame {
    Q_OBJECT

    friend class IOFrameLogic;

public:
    using Ptr = IOFrame*;
    using ConstPtr = const IOFrame*;

private:
    Ui::IOFrame *ui;
    IOFrameLogic* dPtr;

public:
    explicit IOFrame(QWidget *parent = nullptr);
    ~IOFrame();

private:
    void setupUi();
    void setupSignalsAndSlots();

};

#endif // IOFRAME_HPP
