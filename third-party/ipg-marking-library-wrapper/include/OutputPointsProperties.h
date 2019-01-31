#pragma once

#include "IpgmlDef.h"
#include "Enums.h"

namespace ipg_marking_library_wrapper {

    class OutputPointsPropertiesPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API OutputPointsProperties {
    private:
        OutputPointsPropertiesPrivate* dPtr;
    
    private:
        OutputPointsProperties();

    public:
        OutputPointsProperties(float pulseEnergy);
        OutputPointsProperties(float pulseEnergy, Optimization opt);
        OutputPointsProperties(OutputPointsProperties&& other);
        ~OutputPointsProperties();

        Optimization getOptimization() const;
        float getPulseEnergy() const;

        void* getManagedPtr();
        void releaseManagedPtr();

    };

}