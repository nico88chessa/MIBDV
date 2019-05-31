#ifndef IOFRAMELOGIC_HPP
#define IOFRAMELOGIC_HPP

#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>

#include <devices/IOManager.hpp>
#include <IOSignaler.hpp>

class IOFrame;

class IOFrameLogic : public QObject {
    Q_OBJECT

    friend class IOFrame;

public:
    using Ptr = IOFrameLogic*;
    using ConstPtr = const IOFrameLogic*;

private:
    IOFrame* qPtr;
    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    QWeakPointer<PROGRAM_NAMESPACE::IOSignaler> ioSignaler;

public:
    explicit IOFrameLogic(QObject* parent = nullptr);
    ~IOFrameLogic();

    void initialize();

private slots:
    void setDigitalOutput(PROGRAM_NAMESPACE::IOType type);
    void unsetDigitalOutput(PROGRAM_NAMESPACE::IOType type);


};


#endif // IOFRAMELOGIC_HPP
