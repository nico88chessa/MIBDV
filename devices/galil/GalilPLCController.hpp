#ifndef GALILPLCCONTROLLER_HPP
#define GALILPLCCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <Logger.hpp>
#include <devices/galil/GalilControllerUtils.hpp>
#include <data/GalilPLCStatusBean.hpp>
#include "AbstractPLC.hpp"

namespace PROGRAM_NAMESPACE {

class GalilPLCController : public AbstractPLC<GalilPLCStatusBean, int> {
public:
    using Ptr = GalilPLCController *;
    using ConstPtr = const GalilPLCController *;

    static constexpr DeviceKey deviceKey = DeviceKey::GALIL_PLC;

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
    int getRecord(GalilPLCStatusBean& status);

    void setupController(int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    bool connect(const QString& ip);
    virtual int getDigitalInput(int input, int& inputStatus);
    virtual int getDigitalOutput(int output, int& outputStatus);
    virtual int getAnalogInput(int analogInput, anlType& analogInputStatus);
    virtual int setDigitalOutput(int output, bool value);
    int getTCCode(int& tcCode);
    bool isConnected() const;
    virtual GalilPLCStatusBean getStatus();
    virtual bool isError(int errorCode) { return errorCode != G_NO_ERROR; }

private:
    int disconnect();
    inline GCon handle() const { return *this->handler.data(); }
    inline void writeError(int errorCode);
    inline void writeErrorIfExists(int errorCode);
    int getInputs(int bank, int& bankStatus);
    inline void setConnected(bool value) { connected = value; }

};

// type traits

template <>
struct isPLC<GalilPLCController> {
    static constexpr bool value = true;
    using statusType = GalilPLCStatusBean;
    using errorType = int;
};

template <>
struct isDevice<GalilPLCController> {
    static constexpr bool value = true;
    using statusType = GalilPLCStatusBean;
};

}

#endif // GALILPLCCONTROLLER_HPP
