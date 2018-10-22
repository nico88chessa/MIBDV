#ifndef GALILPLCCONTROLLER_HPP
#define GALILPLCCONTROLLER_HPP

#include <gclib.h>
#include <gclibo.h>

#include <QString>
#include <QScopedPointer>

#include <configure.h>
#include <utils/Logger.hpp>
#include <devices/GalilCNControllerUtils.hpp>

namespace PROGRAM_NAMESPACE {

class GalilPLCController {
    using Ptr = GalilPLCController *;
    using ConstPtr = const GalilPLCController *;

    using anlType = double;

private:
    QScopedPointer<GCon> handler;
    bool isInitialized;
    int numDigitalInput;
    int numDigitalOutput;
    int numAnalogInput;

    static constexpr const int NUM_IO_PER_BANK = 8;

public:
    GalilPLCController();
    ~GalilPLCController();
    bool getRecord(GDataRecord47000_ENC& status);

    void setupController(int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    bool connect(const QString& ip);
    bool getDigitalInput(int input, int& inputStatus);
    bool getDigitalOutput(int output, int& outputStatus);
    bool getAnalogInput(int analogInput, anlType& analogInputStatus);
    bool setDigitalOutput(int output, bool value);

private:
    inline GCon handle() { return *this->handler.data(); }
    inline void writeError(int errorCode);
    inline void writeErrorIfExists(int errorCode);
    bool getInputs(int bank, int& bankStatus);

};

}

#endif // GALILPLCCONTROLLER_HPP
