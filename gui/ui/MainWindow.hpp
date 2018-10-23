#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QFile>

#include "UnmovableWindow.hpp"
#include <Logger.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public UnmovableWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
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
