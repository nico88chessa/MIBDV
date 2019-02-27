#pragma once

#include "IpgmlDef.h"
#include "Enums.h"

namespace ipg_marking_library_wrapper {

    class OutputPointsPropertiesWrapperPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API OutputPointsPropertiesWrapper {
    private:
        OutputPointsPropertiesWrapperPrivate* dPtr;

    public:
        OutputPointsPropertiesWrapper(void* obj);
        ~OutputPointsPropertiesWrapper();

        Optimization getOptimization() const;
        float getPulseEnergy() const;

    };

}