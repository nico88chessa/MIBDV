#include <QApplication>
#include <QFontDatabase>
#include <QMetaType>
#include <QThread>

#include <Logger.hpp>
#include <gui/ui/MainWindow.hpp>

#include <MotionBean.hpp>
#include <GalilCNStatusBean.hpp>
#include <GalilPLCStatusBean.hpp>
#include <DigitalInput.hpp>
#include <DigitalOutput.hpp>
#include <AnalogInput.hpp>
#include <DigitalInputValue.hpp>
#include <DigitalOutputValue.hpp>
#include <AnalogInputValue.hpp>

#include <Error.hpp>
#include <Types.hpp>


void loadCustomFont() {

    using namespace PROGRAM_NAMESPACE;

    traceEnter;

    int id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Black");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-BlackItalic");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Bold");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-BoldItalic");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Italic");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Light");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-LightItalic");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Medium");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-MediumItalic");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Regular");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Thin");
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-ThinItalic");

    traceDebug() << "Caricati" << id << " fonts Roboto";

    traceExit;
}

void registerMetatypes() {

    /* NOTE NIC 18/03/2019 - registrazione tipi custom
     * qui vanno registrati i tipi custom che devono essere gestiti
     * dalle connessioni Signal / Slot (soprattutto nel caso di QueueConnection);
     * non e' necessario registrare i tipi se utilizzati solo come QVariant
     * ricordarsi di registrare il tipi sia con namespace sia senza
     * altrimenti le chiamate fatte usando QMetaObject::invokeMethod(...) non funzionano
     */

    using namespace PROGRAM_NAMESPACE;

    traceEnter;
    qRegisterMetaType<PROGRAM_NAMESPACE::GalilCNStatusBean>("GalilCNStatusBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::GalilCNStatusBean>("mibdv::GalilCNStatusBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::GalilPLCStatusBean>("GalilPLCStatusBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::GalilPLCStatusBean>("mibdv::GalilPLCStatusBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::MotionBean>("MotionBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::MotionBean>("mibdv::MotionBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::DeviceKey>("DeviceKey");
    qRegisterMetaType<PROGRAM_NAMESPACE::DeviceKey>("mibdv::DeviceKey");
    qRegisterMetaType<PROGRAM_NAMESPACE::IOType>("IOType");
    qRegisterMetaType<PROGRAM_NAMESPACE::IOType>("mibdv::IOType");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInput>("DigitalInput");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInput>("mibdv::DigitalInput");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutput>("DigitalOutput");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutput>("mibdv::DigitalOutput");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInput>("AnalogInput");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInput>("mibdv::AnalogInput");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInputValue>("DigitalInputValue");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInputValue>("mibdv::DigitalInputValue");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutputValue>("DigitalOutputValue");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutputValue>("mibdv::DigitalOutputValue");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInputValue>("AnalogInputValue");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInputValue>("mibdv::AnalogInputValue");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInputStatus>("DigitalInputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInputStatus>("mibdv::DigitalInputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutputStatus>("DigitalOutputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutputStatus>("mibdv::DigitalOutputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInputStatus>("AnalogInputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInputStatus>("mibdv::AnalogInputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::MotionStopCode>("MotionStopCode");
    qRegisterMetaType<PROGRAM_NAMESPACE::MotionStopCode>("mibdv::MotionStopCode");
    qRegisterMetaType<PROGRAM_NAMESPACE::MachineStatus>("MachineStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::MachineStatus>("mibdv::MachineStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::Error>("Error");
    qRegisterMetaType<PROGRAM_NAMESPACE::Error>("mibdv::Error");
    qRegisterMetaType<PROGRAM_NAMESPACE::ErrorType>("ErrorType");
    qRegisterMetaType<PROGRAM_NAMESPACE::ErrorType>("mibdv::ErrorType");
    qRegisterMetaType<QList<PROGRAM_NAMESPACE::Error>>("QList<Error>");
    qRegisterMetaType<QList<PROGRAM_NAMESPACE::Error>>("mibdv::QList<Error>");

    traceExit;

}

static constexpr char* MAIN_THREAD_NAME = "Main";

int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

    using namespace PROGRAM_NAMESPACE;
    traceInfo() << "START APPLICATIVO" << APPLICATION_NAME;

    /*
    bool test2 = hasMachineStatusReceiver<Prova>::value;
    bool test3 = hasMachineStatusReceiver<int>::value;
    */

    /*
    bool test4 = hasMachineStatusReceiver<GalilCNMotionAnalizer>::value;
    bool test = hasErrorSignaler<int>::value;
    test = hasErrorSignaler<GalilCNMotionAnalizer>::value;
    */

    /*
    using testStatus = isDevice<GalilCNController>::statusType;
    testStatus prova;

    bool v1 = isConnectedDeviceInspector<ConnectedDeviceInspector<GalilCNController>>::value;
    bool v2 = isCN<ConnectedDeviceInspector<GalilPLCController>::type>::value;
    bool v3 = isConnectedDeviceInspector<int>::value;
    bool v4 = isConnectedDeviceInspector<ConnectedDeviceInspector<int>>::value;
    bool isMotionInspectorTC = isCNInspector<ConnectedDeviceInspector<GalilCNController>>();
    bool isMotionInspectorTCK = isCNInspector<ConnectedDeviceInspector<GalilPLCController>>();
    bool isMotionInspectorTP = isPLCInspector<ConnectedDeviceInspector<GalilPLCController>>();
    bool isMotionInspectorTPK = isPLCInspector<ConnectedDeviceInspector<GalilCNController>>();
    bool isMotionInspectorT2 = isCNInspector<int>();
    */

    QApplication app(argc, argv);
    QApplication::instance()->thread()->setObjectName(MAIN_THREAD_NAME);
    QCoreApplication::setOrganizationName(PROGRAM_NAMESPACE::ORGANIZATION);
    QCoreApplication::setApplicationName(PROGRAM_NAMESPACE::APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(PROGRAM_NAMESPACE::APPLICATION_VERSION);

    registerMetatypes();

    loadCustomFont();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();

}
