#ifndef GALILPLCCONTROLLER_HPP
#define GALILPLCCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <utils/Logger.hpp>
#include <devices/GalilCNControllerUtils.hpp>
#include <AbstractDevice.hpp>

namespace PROGRAM_NAMESPACE {

class GalilPLCController : public AbstractDevice<GDataRecord47000_ENC> {
    using Ptr = GalilPLCController *;
    using ConstPtr = const GalilPLCController *;

    using anlType = double;

private:
    QScopedPointer<GCon> handler;
    bool isInitialized;
    bool connected;
    int numDigitalInput;
    int numDigitalOutput;
    int numAnalogInput;

    static constexpr const int NUM_IO_PER_BANK = 8;

public:
    GalilPLCController();
    virtual ~GalilPLCController();
    bool getRecord(GDataRecord47000_ENC& status) const;

    void setupController(int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    bool connect(const QString& ip);
    bool getDigitalInput(int input, int& inputStatus);
    bool getDigitalOutput(int output, int& outputStatus);
    bool getAnalogInput(int analogInput, anlType& analogInputStatus);
    bool setDigitalOutput(int output, bool value);
    bool getTCCode(int& tcCode) const;
    bool isConnected() const;
    GDataRecord47000_ENC getStatus() const;

private:
    inline GCon handle() const { return *this->handler.data(); }
    inline void writeError(int errorCode) const;
    inline void writeErrorIfExists(int errorCode) const;
    bool getInputs(int bank, int& bankStatus);

};

// type traits

template <>
struct isDevice<GalilPLCController> {
    static constexpr bool value = true;
    using statusType = GDataRecord47000_ENC;
};

}

#endif // GALILPLCCONTROLLER_HPP
