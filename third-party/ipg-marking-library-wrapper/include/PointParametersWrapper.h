#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    class PointParametersWrapperPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API PointParametersWrapper {
    private:
        PointParametersWrapperPrivate* dPtr;

    public:
        PointParametersWrapper(void* ptr);
        ~PointParametersWrapper();

        void addLaserEntry(float, float, float, int);
        void clearLaserEntries();
        float getLinkError();
        void setLinkError(float v);
        float getLinkRate();
        void setLinkRate(float v);
        float getLinkSettleTc();
        void setLinkSettleTc(float v);
        int getMode();
        void setMode(int v);
        float getVelocity();
        void setVelocity(float v);
    };

}