#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    class PointParametersPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API PointParameters {
    private:
        PointParametersPrivate* dPtr;
    
    private:
        PointParameters();

    public:
        PointParameters(unsigned int);
        PointParameters(PointParameters&& copy);
        ~PointParameters();

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

        void* getManagedObject();
        void releaseManagedObject();
        
    };

}