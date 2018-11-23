#ifndef MOTIONMANAGER_HPP
#define MOTIONMANAGER_HPP

#include <QObject>
#include <QScopedPointer>

#include <AbstractCN.hpp>

class MotionManager : public QObject {
    Q_OBJECT

public:
    using Ptr = MotionManager*;
    using ConstPtr = const MotionManager*;


public:
    explicit MotionManager(QObject* parent = nullptr);
    ~MotionManager();

    bool moveX(int posMm);

protected:
    virtual void initCN() = 0;

    void waitAxisXStop();

    virtual bool moveXImpl(int posMm) = 0;

signals:
    void axisXStop(int errorCode);

};




#endif // MOTIONMANAGER_HPP
