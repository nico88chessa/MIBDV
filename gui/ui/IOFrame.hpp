#ifndef IOFRAME_HPP
#define IOFRAME_HPP

#include <QFrame>

#include <devices/IOManager.hpp>

class IOFrameLogic;

namespace Ui {
class IOFrame;
}

class IOFrame : public QFrame {
    Q_OBJECT

    friend class IOFrameLogic;
    Q_PROPERTY(int rowMinHeight READ getRowMinHeight WRITE setRowMinHeight)

public:
    using Ptr = IOFrame*;
    using ConstPtr = const IOFrame*;

private:
    static constexpr int IOFRAME_DSB_MIN_VALUE = -100000;
    static constexpr int IOFRAME_DSB_MAX_VALUE = 100000;

    Ui::IOFrame *ui;
    IOFrameLogic* dPtr;
    int rowMinHeight;

    PROGRAM_NAMESPACE::DigitalInputStatus digitalInputStatus;
    PROGRAM_NAMESPACE::DigitalOutputStatus digitalOutputStatus;
    PROGRAM_NAMESPACE::AnalogInputStatus analogInputStatus;

    PROGRAM_NAMESPACE::DigitalInputSet digitalInputs;
    PROGRAM_NAMESPACE::DigitalOutputSet digitalOutputs;
    PROGRAM_NAMESPACE::AnalogInputSet analogInputs;

public:
    explicit IOFrame(QWidget *parent = nullptr);
    ~IOFrame();

    int getRowMinHeight() const;
    void setRowMinHeight(int value);

private:
    void setupUi();
    void setupSignalsAndSlots();

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void updateUI();

public slots:
    void updateDigitalIOStatus(const PROGRAM_NAMESPACE::DigitalInputStatus& iStatus,
                               const PROGRAM_NAMESPACE::DigitalOutputStatus& oStatus,
                               const PROGRAM_NAMESPACE::AnalogInputStatus& aiStatus);

};

#endif // IOFRAME_HPP
