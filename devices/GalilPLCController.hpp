#ifndef GALILPLCCONTROLLER_HPP
#define GALILPLCCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <Logger.hpp>
#include <devices/GalilControllerUtils.hpp>
#include "AbstractPLC.hpp"

namespace PROGRAM_NAMESPACE {

class GalilPLCController : public AbstractPLC<GDataRecord47000_ENC, int> {
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
    int getRecord(GDataRecord47000_ENC& status) const;

    void setupController(int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    bool connect(const QString& ip);
    virtual int getDigitalInput(int input, int& inputStatus);
    virtual int getDigitalOutput(int output, int& outputStatus);
    virtual int getAnalogInput(int analogInput, anlType& analogInputStatus);
    virtual int setDigitalOutput(int output, bool value);
    int getTCCode(int& tcCode) const;
    bool isConnected() const;
    virtual GDataRecord47000_ENC getStatus() const;

private:
    inline GCon handle() const { return *this->handler.data(); }
    inline void writeError(int errorCode) const;
    inline void writeErrorIfExists(int errorCode) const;
    int getInputs(int bank, int& bankStatus);

};

// type traits

template <>
struct isPLC<GalilPLCController> {
    static constexpr bool value = true;
    using statusType = GDataRecord47000_ENC;
    using errorType = int;
};

template <>
struct isDevice<GalilPLCController> {
    static constexpr bool value = true;
    using statusType = GDataRecord47000_ENC;
};

}

#endif // GALILPLCCONTROLLER_HPP
