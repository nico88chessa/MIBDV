#pragma once

#include "IpgmlDef.h"
#include "Enums.h"

namespace ipg_marking_library_wrapper {

    class OutputVectorsPropertiesWrapperPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API OutputVectorsPropertiesWrapper {
    private:
        OutputVectorsPropertiesWrapperPrivate* dPtr;

    public:
        OutputVectorsPropertiesWrapper(void* ptr);
        ~OutputVectorsPropertiesWrapper();

        float getFrequency() const;
        Optimization getOptimization() const;
        float getPitch() const;
        float getPulseEnergy() const;
        float getPulseWidth() const;

    };

}