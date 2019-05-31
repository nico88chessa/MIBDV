#ifndef GALILCNMOTIONANALIZER_HPP
#define GALILCNMOTIONANALIZER_HPP

#include <MotionAnalizer.hpp>
#include <GalilCNStatusBean.hpp>


namespace PROGRAM_NAMESPACE {

class GalilCNMotionAnalizer : public AbstractAnalizer<GalilCNStatusBean> {
private:
    bool isFirst;
    GalilCNStatusBean lastStatus;
    char isFECheck;
    bool isCustomHomeAxisX;

    static constexpr char CUSTOM_HOME_AXIS_X_FE_COUNT = 2;

public:
    GalilCNMotionAnalizer(QObject* parent = nullptr);

protected:
    virtual void analizeImpl(const GalilCNStatusBean& newStatus) override;

};

template <>
struct hasAnalizerTraits<GalilCNStatusBean> {
    static constexpr bool value = true;
    using analizer = GalilCNMotionAnalizer;
};

}

#endif // GALILCNMOTIONANALIZER_HPP
