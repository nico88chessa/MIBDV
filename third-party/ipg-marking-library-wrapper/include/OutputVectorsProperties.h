#pragma once

#include "IpgmlDef.h"
#include "Enums.h"

namespace ipg_marking_library_wrapper {

    class OutputVectorsPropertiesPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API OutputVectorsProperties {
    private:
        OutputVectorsPropertiesPrivate* dPtr;

    private:
        OutputVectorsProperties();

    public:
        OutputVectorsProperties(float pitch, float pulseEnergy);
        OutputVectorsProperties(float powerWatt, float velocity, float frequency, float pulseWidth, Optimization opt);
        OutputVectorsProperties(float pitch, float pulseEnergy, Optimization opt);
        OutputVectorsProperties(float pitch, float pulseEnergy, Optimization opt, float frequency);
        OutputVectorsProperties(OutputVectorsProperties&& other);
        ~OutputVectorsProperties();

        float getFrequency() const;
        Optimization getOptimization() const;
        float getPitch() const;
        float getPulseEnergy() const;
        float getPulseWidth() const;

        void* getManagedPtr();
        void releaseManagedPtr();
        
    };

}

