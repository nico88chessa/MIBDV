#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    using HANDLE_PTR = void*;

    class VectorParametersWrapperPrivate;
    class Scanner;

    class IPGMARKINGLIBRARYWRAPPER_API VectorParametersWrapper {
        friend class Scanner;

    private:
        VectorParametersWrapperPrivate* dPtr;

    private:
        VectorParametersWrapper(HANDLE_PTR obj = nullptr);

    public:
        VectorParametersWrapper(const VectorParametersWrapper& other) = delete;
        VectorParametersWrapper(VectorParametersWrapper&& other);
        ~VectorParametersWrapper();

        VectorParametersWrapper& operator=(const VectorParametersWrapper& other) = delete;
        VectorParametersWrapper& operator=(VectorParametersWrapper&& other);

        void addLaserEntry(float velocity, float frequency, float pulseWidth, float powerPercent);
        void clearLaserEntries();

        float getEndVelocity();
        void setEndVelocity(float value);
        float getLaserOnAdjust();
        void setLaserOnAdjust(float value);
        float getLinkRate();
        void setLinkRate(float value);
        float getLinkSettleTc();
        void setLinkSettleTc(float value);
        float getMaxAccelRatio();
        void setMaxAccelRatio(float value);
        float getMaxRadialError();
        void setMaxRadialError(float value);
        float getMidVelocity();
        void setMidVelocity(float value);
        int getMode();
        void setMode(int value);
        float getVertexSettleTc();
        void setVertexSettleTc(float value);


    };

}