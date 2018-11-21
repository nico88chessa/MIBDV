#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QScopedPointer>

#include "UnmovableWindow.hpp"

#include <ErrorManager.hpp>
#include <GalilCNInspector.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    QScopedPointer<PROGRAM_NAMESPACE::ErrorManager> errorManager;
    QScopedPointer<PROGRAM_NAMESPACE::GalilCNInspector> galilCNInspector;

    QScopedPointer<QThread> galilCNInspectorThread;

public:

private:
    void setupSignalsAndSlots() const;
    void initPanels();
    void initLeftPanel();
    void initContentPanel();

    void initDevices();

    void startGalilCNInspector();
    void stopGalilCNInspector();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupStyleSheets() const;
    void startDevices();

signals:
    void galilCNStatusUpdateSignal(const PROGRAM_NAMESPACE::GalilCNStatusBean& status);

};

#endif // MAINWINDOW_HPP
