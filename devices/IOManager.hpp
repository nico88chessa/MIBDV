#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include <QObject>
#include <QHash>

#include <AbstractDevice.hpp>
#include <CommonTraits.hpp>
#include <type_traits>
#include <GalilCNController.hpp>
#include <GalilPLCController.hpp>


namespace PROGRAM_NAMESPACE {

class IOManager : public QObject {
    Q_OBJECT

private:

public:
    explicit IOManager(QObject *parent = nullptr);

private:
    template<DeviceKey D>
    void updateStatus(const void* status) {

        static_assert(deviceKeyTraits<D>::value, "");
        using deviceType = typename deviceKeyTraits<D>::type;
        static_assert(isDevice<deviceType>::value, "");
        using statusType = typename isDevice<deviceType>::statusType;

        statusType s = *static_cast<const statusType*>(status);


    }


public slots:

    void setStatus(DeviceKey k, const void* status);


signals:

};

}

#endif // IOMANAGER_HPP
