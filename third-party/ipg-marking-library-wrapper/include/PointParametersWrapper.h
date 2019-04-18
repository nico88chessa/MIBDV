#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    using POINT_PARAMETERS_WRAPPER_HANDLE_PTR = void*;
    using CONST_POINT_PARAMETERS_WRAPPER_HANDLE_PTR = const void*;

    class PointParametersWrapperPrivate;
    class Scanner;

    class IPGMARKINGLIBRARYWRAPPER_API PointParametersWrapper {
        friend class Scanner;

    private:
        PointParametersWrapperPrivate* dPtr;

    private:
        PointParametersWrapper(POINT_PARAMETERS_WRAPPER_HANDLE_PTR obj = nullptr);

    public:
        PointParametersWrapper(const PointParametersWrapper& other) = delete;
        PointParametersWrapper(PointParametersWrapper&& other);
        ~PointParametersWrapper();

        PointParametersWrapper& operator=(const PointParametersWrapper& other) = delete;
        PointParametersWrapper& operator=(PointParametersWrapper&& other);

        void addLaserEntry(float dwell, float width, float powerPercent, int count);
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