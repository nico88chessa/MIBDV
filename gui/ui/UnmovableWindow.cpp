#include "UnmovableWindow.hpp"

UnmovableWindow::UnmovableWindow(QWidget *parent) :
    QMainWindow(parent) {
    //, rect(0, 0, 1024, 768) {

}

void UnmovableWindow::moveEvent(MAYBE_UNUSED QMoveEvent* event) {
    this->setGeometry(rect);
}
