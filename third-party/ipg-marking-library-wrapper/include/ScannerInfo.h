#pragma once

#include "IpgmlDef.h"

#include <iostream>
#include "Enums.h"

namespace ipg_marking_library_wrapper {

    //class ScannerInfoPrivate;

    class ScannerInfo {
    private:
        ConnectionStatus status;
        std::string name;

    public:
        IPGMARKINGLIBRARYWRAPPER_API ScannerInfo(ConnectionStatus status, const std::string& name);
        IPGMARKINGLIBRARYWRAPPER_API ~ScannerInfo();
        IPGMARKINGLIBRARYWRAPPER_API ConnectionStatus getStatus() const;
        IPGMARKINGLIBRARYWRAPPER_API std::string getName() const;
    };

}