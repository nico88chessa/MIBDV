#ifndef ALERTFRAME_HPP
#define ALERTFRAME_HPP

#include <QFrame>


class AlertFrameLogic;
class AlertItemDelegate;
class AlertItemModel;

namespace Ui {
class AlertFrame;
}

class AlertFrame : public QFrame {
    Q_OBJECT
    friend class AlertFrameLogic;
    friend class AlertItemDelegate;

public:
    using Ptr = AlertFrame*;
    using ConstPtr = const AlertFrame*;

private:
    Ui::AlertFrame *ui;
    AlertFrameLogic* dPtr;
    AlertItemDelegate* delegate;

public:
    explicit AlertFrame(QWidget *parent = nullptr);
    ~AlertFrame();

private:
    void setupUi();
    void setupSignalsAndSlots();

};

#endif // ALERTFRAME_HPP
