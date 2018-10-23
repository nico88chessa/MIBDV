#ifndef UNMOVABLEWINDOW_HPP
#define UNMOVABLEWINDOW_HPP

#include <QMainWindow>

#include <configure.h>

class UnmovableWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit UnmovableWindow(QWidget *parent = nullptr);

private:
    QRect rect;

protected:
    void moveEvent(QMoveEvent* event);

};

#endif // UNMOVABLEWINDOW_HPP
