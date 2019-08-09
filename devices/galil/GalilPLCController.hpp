#ifndef GALILPLCCONTROLLER_HPP
#define GALILPLCCONTROLLER_HPP

#include <QString>
#include <QScopedPointer>

#include <configure.h>
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
    bool connectionStatus;
    int numDigitalInput;
    int numDigitalOutput;
    int numAnalogInput;
    QString handleCode;
    QString ipAddress;
    int commandTimeoutMs;

    static constexpr const int NUM_IO_PER_BANK = 8;

public:
    GalilPLCController();
    virtual ~GalilPLCController() override;
    int getRecord(GalilPLCStatusBean& status);

    void setupController(const QString& ipAddress,
                         int commandTimeoutMs,
                         int numDigitalInput,
                         int numDigitalOutput,
                         int numAnalogInput);
    virtual bool connect() override;
    virtual int getDigitalInput(int input, int& inputStatus) override;
    virtual int getDigitalOutput(int output, int& outputStatus) override;
    virtual int getAnalogInput(int analogInput, anlType& analogInputStatus) override;
    virtual int setDigitalOutput(int output, bool value) override;
    int getTCCode(int& tcCode);
    virtual bool isConnected() override;
    virtual GalilPLCStatusBean getStatus() override;
    virtual bool isError(int errorCode) override { return errorCode != G_NO_ERROR; }
    virtual QString decodeError(const int& errorCode) override;
    int getKeepAliveTimeMs(unsigned int* timeMs, unsigned int* newValue = nullptr); // 10 min valore default

private:
    inline bool getConnectionStatus() const { return connectionStatus; }
    inline void setConnectionStatus(bool value) { connectionStatus = value; }
    int disconnect();
    inline GCon handle() const { return *this->handler.data(); }
    inline void writeError(int errorCode);
    inline void writeErrorIfExists(int errorCode);
    int getInputs(int bank, int& bankStatus);

};

}

#endif // GALILPLCCONTROLLER_HPP
