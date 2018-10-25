#include <QApplication>

#include <gui/ui/MainWindow.hpp>

int main(MAYBE_UNUSED int argc, MAYBE_UNUSED char** argv) {

    using namespace PROGRAM_NAMESPACE;

    traceInfo() << "START APPLICATIVO" << APPLICATION_NAME;

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(PROGRAM_NAMESPACE::ORGANIZATION);
    QCoreApplication::setApplicationName(PROGRAM_NAMESPACE::APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(PROGRAM_NAMESPACE::APPLICATION_VERSION);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();

}
