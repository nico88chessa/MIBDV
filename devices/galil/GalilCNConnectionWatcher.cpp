#include "GalilCNConnectionWatcher.hpp"
#include "Settings.hpp"
#include "Logger.hpp"
#include "GalilCNController.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNConnectionWatcher::GalilCNConnectionWatcher(QObject* parent) :
    AbstractDeviceConnectionWatcher(parent),
    timerKeepAlive(this) {

    int intervalMs = Settings::instance().getGalilCNCheckConnectionIntervalMs();
    this->setupTimers(intervalMs);

    connect(&timerKeepAlive, &QTimer::timeout, [this]() {
        traceEnter;
        unsigned int timeMs;
        device.staticCast<GalilCNController>()->getKeepAliveTimeMs(&timeMs);
        traceExit;
    });

}

GalilCNConnectionWatcher::~GalilCNConnectionWatcher() {

    traceEnter;
    timerKeepAlive.stop();
    traceExit;

}

void GalilCNConnectionWatcher::reconnectDevice() {

    traceEnter;
    if (!device.isNull()) {

        if (!device->isConnected()) {

            timerKeepAlive.stop();

            Settings& s = Settings::instance();
            QString ipCN = s.getGalilCNIpAddress();
            auto galilCN = device.staticCast<GalilCNController>().data();
            galilCN->setupController(
                        s.getGalilCNIpAddress(),
                        s.getGalilCNNumberDigitalInput(),
                        s.getGalilCNNumberDigitalOutput(),
                        s.getGalilCNNumberAnalogInput(),
                        s.getGalilCNOptionCustomHomeAxisX());

            if (galilCN->connect()) {
                unsigned int timeMs;
                int res = galilCN->getKeepAliveTimeMs(&timeMs);
                if (galilCN->isError(res)) {

                    traceErr() << "Errore chiamata CN keep alive; codice errore:" << res;
                    traceErr() << "Descrizione errore:" << galilCN->decodeError(res);

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
