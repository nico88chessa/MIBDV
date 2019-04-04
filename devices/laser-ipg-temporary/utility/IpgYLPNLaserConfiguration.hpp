#ifndef IPGYLPNLASERCONFIGURATION_HPP
#define IPGYLPNLASERCONFIGURATION_HPP

#include <QList>


class IpgYLPNLaserConfiguration {
public:
    using Ptr = IpgYLPNLaserConfiguration*;
    using ConstPtr = const IpgYLPNLaserConfiguration*;

public:
    struct Mode {
        float pulseDuration;
        float nominalFrequency;
        float minFrequency;
        float maxFrequency;
    };

private:
    QList<Mode> modes;
    bool isInitialized;
    int currentModeIndex;
    int currentFrequency;
    float currentPower;

private:
    IpgYLPNLaserConfiguration() : isInitialized(false) { }
    ~IpgYLPNLaserConfiguration() { }

public:
    IpgYLPNLaserConfiguration(const IpgYLPNLaserConfiguration&) = delete;
    void operator=(const IpgYLPNLaserConfiguration&) = delete;
    void operator=(IpgYLPNLaserConfiguration&&) = delete;

    static IpgYLPNLaserConfiguration& instance() {
        static IpgYLPNLaserConfiguration configuration;
        return configuration;
    }

    void addMode(float pulseDuration, float nominalFrequency, float minFrequency, float maxFrequency) {

        Mode newMode;
        newMode.pulseDuration = pulseDuration;
        newMode.minFrequency = minFrequency;
        newMode.maxFrequency = maxFrequency;
        newMode.nominalFrequency = nominalFrequency;
        modes.append(newMode);
    }

    const Mode& getMode(int index) { return modes.at(index); }
    const QList<Mode>& getModes() { return this->modes; }

    bool getIsInitialized() const { return isInitialized; }
    void setIsInitialized(bool value) { isInitialized = value; }

    int getCurrentFrequency() const { return currentFrequency; }
    void setCurrentFrequency(int value) { currentFrequency = value; }

    float getCurrentPower() const { return currentPower; }
    void setCurrentPower(float value) { currentPower = value; }

    int getCurrentModeIndex() const { return currentModeIndex; }
    void setCurrentModeIndex(int value) { currentModeIndex = value; }

};

#endif // IPGYLPNLASERCONFIGURATION_HPP
