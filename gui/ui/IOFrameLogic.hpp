#ifndef IOFRAMELOGIC_HPP
#define IOFRAMELOGIC_HPP

#include <QObject>

class IOFrame;

class IOFrameLogic : public QObject {
    Q_OBJECT

    friend class IOFrame;

public:
    using Ptr = IOFrameLogic*;
    using ConstPtr = const IOFrameLogic*;

private:
    IOFrame* qPtr;

public:
    explicit IOFrameLogic(QObject* parent = nullptr);
    ~IOFrameLogic();



};


#endif // IOFRAMELOGIC_HPP
