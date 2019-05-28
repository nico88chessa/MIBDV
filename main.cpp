#include <QApplication>
#include <QFontDatabase>

#include <QThread>

#include <Settings.hpp>
#include <Logger.hpp>
#include <gui/ui/MainWindow.hpp>

#include <MotionBean.hpp>
#include <GalilCNStatusBean.hpp>
#include <GalilPLCStatusBean.hpp>
#include <DigitalInputValue.hpp>
#include <IOSignaler.hpp>
#include <Types.hpp>

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DeviceKey)

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

    // NOTE NIC 18/03/2019 - qui vanno registrati i tipi custom che devono essere gestiti
    // dalle connessioni Signal / Slot
    // Non e' necessario registrare i tipi se utilizzati solo come QVariant

    using namespace PROGRAM_NAMESPACE;

    traceEnter;

    qRegisterMetaType<PROGRAM_NAMESPACE::GalilCNStatusBean>("GalilCNStatusBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::GalilPLCStatusBean>("GalilPLCStatusBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::MotionBean>("MotionBean");
    qRegisterMetaType<PROGRAM_NAMESPACE::DeviceKey>("DeviceKey");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalInputStatus>("DigitalInputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::DigitalOutputStatus>("DigitalOutputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::AnalogInputStatus>("AnalogInputStatus");
    qRegisterMetaType<PROGRAM_NAMESPACE::MotionStopCode>("MotionStopCode");

    traceExit;

}

//int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

//    using namespace PROGRAM_NAMESPACE;
//    traceInfo() << "START APPLICATIVO" << APPLICATION_NAME;

////    QFile file("C:\\Users\\nicola\\Desktop\\F15-60x60-ordinato - Copia.json");
//    QFile file("C:\\Users\\nicola\\Desktop\\a4-2 - Copia.json");

////    QFile file("C:\\Users\\nicola\\Desktop\\test.json");
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        traceErr() << "Impossibile aprire il file";
//        exit(-1);
//    }

//    QString array = file.readAll();
//    file.close();

//    IAbstractJsonParser::Ptr parser = new FilterJsonParser();
//    Filter test;
//    parser->decodeJson(array.toUtf8(), &test);
//    QByteArray temp = array.toUtf8();



////    QJsonDocument doc = QJsonDocument::fromJson(array.toUtf8());
////    if (doc.isNull()) {
////        traceErr() << "Err parsing json";
////        exit(-1);
////    }

////    QJsonObject obj = doc.object();
////    for (auto&& it=obj.constBegin(); it!=obj.constEnd(); ++it) {
////        traceDebug() << "test";
////        exit(-1);
////    }


//}

static constexpr char* MAIN_THREAD_NAME = "Main";

int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

    using namespace PROGRAM_NAMESPACE;
    traceInfo() << "START APPLICATIVO" << APPLICATION_NAME;

//    using testStatus = isDevice<GalilCNController>::statusType;
//    testStatus prova;
//
//    bool v1 = isConnectedDeviceInspector<ConnectedDeviceInspector<GalilCNController>>::value;
//    bool v2 = isCN<ConnectedDeviceInspector<GalilPLCController>::type>::value;
//    bool v3 = isConnectedDeviceInspector<int>::value;
////    bool v4 = isConnectedDeviceInspector<ConnectedDeviceInspector<int>>::value;
    //bool isMotionInspectorTC = isCNInspector<ConnectedDeviceInspector<GalilCNController>>();
    //bool isMotionInspectorTCK = isCNInspector<ConnectedDeviceInspector<GalilPLCController>>();
//    bool isMotionInspectorTP = isPLCInspector<ConnectedDeviceInspector<GalilPLCController>>();
//    bool isMotionInspectorTPK = isPLCInspector<ConnectedDeviceInspector<GalilCNController>>();
    //bool isMotionInspectorT2 = isCNInspector<int>();

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


//int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

//    using namespace PROGRAM_NAMESPACE;

//    GalilControllerUtils::decodeError(G_CUSTOM_CN_DIGITAL_INPUT_OUT_OF_RANGE);

//    traceInfo() << "START APPLICATIVO" << APPLICATION_NAME;
//    traceEnter;


//    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
//    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
//    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);
//    traceInfo() << QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);

//    traceExit;

//    GalilCNController test;
//    traceDebug() << isDevice<AbstractDevice<GDataRecord2103> >::value;
//    traceDebug() << isDevice<GalilCNController>::value;
//    traceDebug() << isDevice<GalilPLCController>::value;

//    GalilCNInspector inspector;
//    ErrorManager errorManager;
//    errorManager.subscribeObject(inspector);

//    test.connect("169.254.12.10");
//    test.setupController(16, 16, 8);

//    static_assert(isDevice<decltype (test)>::value, "non sono un  device");
//    using status = isDevice<decltype (test)>::statusType;
//    status a;
//    a = test.getStatus();

//    traceDebug() << "isCN:" << isCN<int>::value;
//    traceDebug() << "isCN:" << isCN<GalilCNController>::value;
//    traceDebug() << "isCN:" << isCN<GalilPLCController>::value;

//    traceDebug() << "isPLC:" << isPLC<int>::value;
//    traceDebug() << "isPLC:" << isPLC<GalilCNController>::value;
//    traceDebug() << "isPLC:" << isPLC<GalilPLCController>::value;

//    int st;
//    double v;
//    for (int i=0; i<16; ++i) {
//        test.getDigitalInput(i+1, st);
//        traceDebug() << i+1 << ":" << st;
//    }
//    for (int i=0; i<16; ++i) {
//        test.getDigitalOutput(i+1, st);
//        traceDebug() << i+1 << ":" << st;
//    }
//    for (int i=0; i<8; ++i) {
//        test.getAnalogInput(i+1, v);
//        traceDebug() << i+1 << ":" << v;
//    }

//    bool b;
//    test.getPosition(GalilCNController::Axis::X, st); traceInfo() << st;
//    test.isAxisInMotion(GalilCNController::Axis::X, b); traceInfo() << b;
//    test.isAxisPositionError(GalilCNController::Axis::X, b); traceInfo() << b;
//    test.isMotorOff(GalilCNController::Axis::X, b); traceInfo() << b;
//    test.isForwardLimit(GalilCNController::Axis::X, b); traceInfo() << b;
//    test.isBackwardLimit(GalilCNController::Axis::X, b); traceInfo() << b;
//    test.isHomeAxis(GalilCNController::Axis::X, b); traceInfo() << b;
//    test.checkAbort(b); traceInfo() << b;
//    test.setDigitalOutput(5, true);
//    test.setSpeeds(GalilCNController::Axis::X, 10);
//    test.setAccelerations(GalilCNController::Axis::X, 2048, 3072);
//    test.setMoveParameters(GalilCNController::Axis::X, 10, 2048, 3072);
//    test.stopAxis(GalilCNController::Axis::X);
//    test.homingX(1024, 2048, 3072);
//    test.homingY(1024, 2048, 3072);
//    test.homingZ(1024, 2048, 3072);
//    test.homingW(1024, 2048, 3072);
//    test.home(GalilCNController::Axis::X, 1024, 2048, 3072);
//    test.setPosition(GalilCNController::Axis::X, 0);
//    test.moveToPosition(GalilCNController::Axis::X, 10000, 1024, 2048, 3072);
//    test.startMoveAxis(GalilCNController::Axis::X);

//    Settings::instance();

//    traceExit;

//    return 0;

//}

