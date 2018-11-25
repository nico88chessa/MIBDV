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
    Q_PROPERTY(int rowMinHeight READ getRowMinHeight WRITE setRowMinHeight)

public:
    using Ptr = IOFrame*;
    using ConstPtr = const IOFrame*;

private:
    Ui::IOFrame *ui;
    IOFrameLogic* dPtr;
    int rowMinHeight;

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
};

#endif // IOFRAME_HPP
