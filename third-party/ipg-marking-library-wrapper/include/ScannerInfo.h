#pragma once

#include "IpgmlDef.h"

#include <iostream>
#include "Enums.h"

namespace ipg_marking_library_wrapper {

    //class ScannerInfoPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API ScannerInfo {
    private:
        ConnectionStatus status;
        std::string name;

    public:
        ScannerInfo(ConnectionStatus status, const std::string& name);
        ~ScannerInfo();
        ConnectionStatus getStatus() const;
        std::string getName() const;
    };

}