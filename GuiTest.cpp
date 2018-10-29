#include <QApplication>
#include <QFontDatabase>

#include <gui/ui/MainWindow.hpp>

int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

    using namespace PROGRAM_NAMESPACE;

    traceInfo() << "START APPLICATIVO" << APPLICATION_NAME;

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(PROGRAM_NAMESPACE::ORGANIZATION);
    QCoreApplication::setApplicationName(PROGRAM_NAMESPACE::APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(PROGRAM_NAMESPACE::APPLICATION_VERSION);

    QFile file(":/fonts/Roboto-Black.ttf");
    qDebug() << file.exists();

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    int id = 0;
    id = QFontDatabase::addApplicationFont(":/fonts/Roboto-Black");
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

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();

}
