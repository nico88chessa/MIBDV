#include "IOManager.hpp"

using namespace PROGRAM_NAMESPACE;

IOManager::IOManager(QObject *parent) : QObject(parent) {

}

void IOManager::setStatus(DeviceKey k, const void* status) {

    switch (k) {
        case DeviceKey::GALIL_CN: this->updateStatus<DeviceKey::GALIL_CN>(status); break;
        case DeviceKey::GALIL_PLC: this->updateStatus<DeviceKey::GALIL_PLC>(status); break;
        case DeviceKey::NONE: break;
    }
    return;

}
