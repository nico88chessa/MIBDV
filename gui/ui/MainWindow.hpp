#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QScopedPointer>

#include "UnmovableWindow.hpp"

#include <galil/GalilCNInspector.hpp>
#include <galil/GalilPLCInspector.hpp>
#include <IOInspector.hpp>

#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <DeviceConnectionWatcher.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    QScopedPointer<PROGRAM_NAMESPACE::ErrorManager> errorManager;

    QScopedPointer<PROGRAM_NAMESPACE::GalilCNInspector> galilCNInspector;
    QScopedPointer<PROGRAM_NAMESPACE::GalilPLCInspector> galilPLCInspector;
    QScopedPointer<PROGRAM_NAMESPACE::IOInspector> ioInspector;

    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;

    QSharedPointer<PROGRAM_NAMESPACE::IAbstractDevice> cn; // utilizzato da motionManager
    QSharedPointer<PROGRAM_NAMESPACE::IAbstractDevice> plc; // utilizzato da motionManager e ioManager
    QScopedPointer<PROGRAM_NAMESPACE::DeviceConnectionWatcher> cnConnectionWatcher;
    QScopedPointer<PROGRAM_NAMESPACE::DeviceConnectionWatcher> plcConnectionWatcher;

public:

private:
    void setupUi();
    void setupSignalsAndSlots() const;
    void setupUiPanels();
    void setupUiLeftPanel();
    void setupUiContentPanel();

    void initDevices();
    void initGalilCNInspector();
    void initGalilPLCInspector();
    void initIOInspector();
    void initCNConnectionWatcher();
    void initPLCConnectionWatcher();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

private slots:
    void setupStyleSheets() const;
    void startDevices();
    void stopDevices();

signals:
    void galilCNStatusUpdateSignal(const PROGRAM_NAMESPACE::GalilCNStatusBean status);
    void galilPLCStatusUpdateSignal(const PROGRAM_NAMESPACE::GalilPLCStatusBean status);
    void ioStatusUpdateSignal(const PROGRAM_NAMESPACE::IOInspector::DigitalInputStatus& digitalInputs,
                              const PROGRAM_NAMESPACE::IOInspector::DigitalOutputStatus& digitalOutputs,
                              const PROGRAM_NAMESPACE::IOInspector::AnalogInputStatus& analogInputs);

};

#endif // MAINWINDOW_HPP
