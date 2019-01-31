#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    class PointWrapperPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API PointWrapper {
    private:
        PointWrapperPrivate* dPtr;

    public:
        PointWrapper(void* obj);
        ~PointWrapper();

        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getZ() const;
        void setZ(float z);

    };

}
