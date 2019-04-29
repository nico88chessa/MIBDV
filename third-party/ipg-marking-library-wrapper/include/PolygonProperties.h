#pragma once

#include "IpgmlDef.h"
#include <iostream>


namespace ipg_marking_library_wrapper {

    class PolygonPropertiesPrivate;

    using POLYGONPROPERTIES_HANDLER = void*;
    using CONST_POLYGONPROPERTIES_HANDLER = const void*;

    class IPGMARKINGLIBRARYWRAPPER_API PolygonProperties {
        friend class PointList;
        friend class VectorList;

    private:
        PolygonPropertiesPrivate* dPtr;

    private:
        CONST_POLYGONPROPERTIES_HANDLER getManagedPtr() const;

    public:
        PolygonProperties(int numberOfSides, float radius);
        ~PolygonProperties();
        float getNumberOfSides() const;
        float getRadius() const;

        /*void* getManagedPtr();
        void releaseManagedPtr();*/

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const PolygonProperties& obj);

    };

}
