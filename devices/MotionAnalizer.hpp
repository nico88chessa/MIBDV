#ifndef MOTIONANALIZER_HPP
#define MOTIONANALIZER_HPP

#include <QObject>

#include <configure.h>


namespace PROGRAM_NAMESPACE {

class MotionSignaler;

class IMotionAnalizer : public QObject {
    Q_OBJECT

public:
    using Ptr = IMotionAnalizer *;
    using ConstPtr = const IMotionAnalizer *;

protected:
    MotionSignaler* signaler;

public:
    explicit IMotionAnalizer(QObject* parent = nullptr) :
        QObject(parent), signaler(nullptr) { }

    void setSignaler(MotionSignaler* const s) {
        this->signaler = s;
    }

protected:
    virtual void analizeImpl(const QVariant& status) = 0;

public slots:
    void analize(const QVariant& status) {
        analizeImpl(status);
    }

};


template <typename T>
class AbstractAnalizer : public IMotionAnalizer {
public:
    using Ptr = AbstractAnalizer*;
    using ConstPtr = const AbstractAnalizer*;

protected:
    virtual void analizeImpl(const T& newStatus) = 0;

public:
    AbstractAnalizer(QObject* parent = nullptr) :
        IMotionAnalizer(parent) { }

    virtual void analizeImpl(const QVariant& status) override final {
        auto obj = qvariant_cast<T>(status);
        analizeImpl(obj);
        emit signaler->motionBeanSignal(MotionBean(obj));
    }

};


// TRAITS

template <typename T>
struct hasAnalizerTraits {
    static constexpr bool value = false;
};

}

#endif // MOTIONSIGNALER_HPP
