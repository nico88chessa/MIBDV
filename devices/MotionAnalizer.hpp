#ifndef MOTIONANALIZER_HPP
#define MOTIONANALIZER_HPP

#include <QObject>

#include <GalilCNStatusBean.hpp>
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
    explicit IMotionAnalizer(QObject* parent = nullptr);

    void setSignaler(MotionSignaler* const s);

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


class GalilMotionAnalizer : public AbstractAnalizer<GalilCNStatusBean> {
private:
    bool isFirst;
    GalilCNStatusBean lastStatus;
    char isFECheck;
    bool isCustomHomeAxisX;

    static constexpr char CUSTOM_HOME_AXIS_X_FE_COUNT = 2;

public:
    GalilMotionAnalizer(QObject* parent = nullptr);

protected:
    virtual void analizeImpl(const GalilCNStatusBean& newStatus) override;

};


// TRAITS

template <typename T>
struct hasAnalizerTraits {
    static constexpr bool value = false;
};

template <>
struct hasAnalizerTraits<GalilCNStatusBean> {
    static constexpr bool value = true;
    using analizer = GalilMotionAnalizer;
};

}

#endif // MOTIONSIGNALER_HPP
