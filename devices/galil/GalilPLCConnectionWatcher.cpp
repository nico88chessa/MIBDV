#include "GalilPLCConnectionWatcher.hpp"
#include "Settings.hpp"
#include "Logger.hpp"
#include "GalilPLCController.hpp"

using namespace PROGRAM_NAMESPACE;

GalilPLCConnectionWatcher::GalilPLCConnectionWatcher(QObject* parent) :
    AbstractDeviceConnectionWatcher(parent),
    timerKeepAlive(this) {

    int intervalMs = Settings::instance().getGalilPLCCheckConnectionIntervalMs();
    this->setupTimers(intervalMs);

    connect(&timerKeepAlive, &QTimer::timeout, [this]() {
        traceEnter;
        unsigned int timeMs;
        device.staticCast<GalilPLCController>()->getKeepAliveTimeMs(&timeMs);
        traceExit;
    });

}

GalilPLCConnectionWatcher::~GalilPLCConnectionWatcher() {

    traceEnter;
    timerKeepAlive.stop();
    traceExit;

}

void GalilPLCConnectionWatcher::reconnectDevice() {

    traceEnter;
    if (!device.isNull()) {

        if (!device->isConnected()) {

            timerKeepAlive.stop();

            Settings& s = Settings::instance();
            QString ipPLC = s.getGalilPLCIpAddress();
            auto galilPLC = device.staticCast<GalilPLCController>().data();
            galilPLC->setupController(
                        s.getGalilPLCIpAddress(),
                        s.getGalilPLCNumberDigitalInput(),
                        s.getGalilPLCNumberDigitalOutput(),
                        s.getGalilPLCNumberAnalogInput());

            if (galilPLC->connect()) {
                unsigned int timeMs;
                int res = galilPLC->getKeepAliveTimeMs(&timeMs);
                if (galilPLC->isError(res)) {

                    traceErr() << "Errore chiamata PLC keep alive; codice errore:" << res;
                    traceErr() << "Descrizione errore:" << galilPLC->decodeError(res);

                } else {

                    timerKeepAlive.setInterval(timeMs / 2);
                    timerKeepAlive.start();
                }
            }
        }
    }

    traceExit;
    return;

}
