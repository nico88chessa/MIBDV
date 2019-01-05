#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QSharedPointer>
#include <QScopedPointer>

#include "UnmovableWindow.hpp"

#include <devices/motion/IOInspector.hpp>

#include <devices/motion/IOManager.hpp>
#include <devices/motion/MotionManager.hpp>
#include <ErrorManager.hpp>
#include <devices/AbstractDeviceConnectionWatcher.hpp>
#include <devices/motion/AbstractMotionInspector.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    QScopedPointer<PROGRAM_NAMESPACE::ErrorManager> errorManager;

    QScopedPointer<PROGRAM_NAMESPACE::AbstractMotionInspector> motionInspector;
    QScopedPointer<PROGRAM_NAMESPACE::IOInspector> ioInspector;
    QScopedPointer<PROGRAM_NAMESPACE::AbstractConnectedDeviceInspector> galilPLCInspector;

    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;

    QSharedPointer<PROGRAM_NAMESPACE::IAbstractDevice> cn; // utilizzato da motionManager
    QSharedPointer<PROGRAM_NAMESPACE::IAbstractDevice> plc; // utilizzato da motionManager e ioManager
    QScopedPointer<PROGRAM_NAMESPACE::AbstractDeviceConnectionWatcher> cnConnectionWatcher;
    QScopedPointer<PROGRAM_NAMESPACE::AbstractDeviceConnectionWatcher> plcConnectionWatcher;

public:

private:
    void setupUi();
    void setupSignalsAndSlots() const;
    void setupUiPanels();
    void setupUiLeftPanel();
    void setupUiContentPanel();

    void initDevices();
    void initMotionInspector();
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
    void motionStatusUpdateSignal(const QVariant& status);
    void galilPLCStatusUpdateSignal(const QVariant& status);
    void ioStatusUpdateSignal(const PROGRAM_NAMESPACE::DigitalInputStatus& digitalInputs,
                              const PROGRAM_NAMESPACE::DigitalOutputStatus& digitalOutputs,
                              const PROGRAM_NAMESPACE::AnalogInputStatus& analogInputs);

};

#endif // MAINWINDOW_HPP
