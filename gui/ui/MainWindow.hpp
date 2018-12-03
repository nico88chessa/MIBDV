#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QScopedPointer>

#include "UnmovableWindow.hpp"

#include <galil/GalilCNInspector.hpp>
#include <galil/GalilPLCInspector.hpp>

#include <IOManager.hpp>
#include <MotionManager.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    QScopedPointer<PROGRAM_NAMESPACE::ErrorManager> errorManager;

//    QScopedPointer<QThread> galilCNInspectorThread;
    QScopedPointer<PROGRAM_NAMESPACE::GalilCNInspector> galilCNInspector;
//    QScopedPointer<QThread> galilPLCInspectorThread;
    QScopedPointer<PROGRAM_NAMESPACE::GalilPLCInspector> galilPLCInspector;

    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;

    QSharedPointer<PROGRAM_NAMESPACE::IAbstractDevice> cn; // utilizzato da motionManager
    QSharedPointer<PROGRAM_NAMESPACE::IAbstractDevice> plc; // utilizzato da motionManager e ioManager

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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

private slots:
    void setupStyleSheets() const;
    void startDevices();
    void stopDevices();

signals:
    void galilCNStatusUpdateSignal(const PROGRAM_NAMESPACE::GalilCNStatusBean& status);
    void galilPLCStatusUpdateSignal(const PROGRAM_NAMESPACE::GalilPLCStatusBean& status);

};

#endif // MAINWINDOW_HPP
