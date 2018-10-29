#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QFile>

#include "UnmovableWindow.hpp"
#include <Logger.hpp>
#include <ErrorManager.hpp>

#include <GalilCNInspector.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    PROGRAM_NAMESPACE::ErrorManager errorManager;
    PROGRAM_NAMESPACE::GalilCNInspector inspector;

public:

private:
    void setupSignalsAndSlots() const;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupStyleSheets() const;

};

#endif // MAINWINDOW_HPP
