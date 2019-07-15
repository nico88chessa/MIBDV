#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QSharedPointer>
#include <QScopedPointer>

#include "UnmovableWindow.hpp"

//#include <MachineStatusHandler.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

//    QScopedPointer<PROGRAM_NAMESPACE::ErrorManager> errorManager;
//    QScopedPointer<PROGRAM_NAMESPACE::AbstractDeviceConnectionWatcher> cnConnectionWatcher;
//    QScopedPointer<PROGRAM_NAMESPACE::AbstractDeviceConnectionWatcher> plcConnectionWatcher;

private:
    void setupUi();
    void setupSignalsAndSlots() const;
    void setupUiPanels();
    void setupUiLeftPanel();
    void setupUiContentPanel();

//    void initCNConnectionWatcher();
//    void initPLCConnectionWatcher();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//private:
//    QSharedPointer<PROGRAM_NAMESPACE::MachineStatusNotifier> msn;

private slots:
    void setupStyleSheets() const;

};

#endif // MAINWINDOW_HPP
