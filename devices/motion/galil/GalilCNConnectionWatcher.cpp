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

            QString ipCN = Settings::instance().getGalilCNIpAddress();
            auto galilCN = device.staticCast<GalilCNController>().data();

            if (galilCN->connect(ipCN)) {
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
