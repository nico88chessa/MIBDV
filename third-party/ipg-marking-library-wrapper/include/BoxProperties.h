#pragma once

#include "IpgmlDef.h"
#include <iostream>


namespace ipg_marking_library_wrapper {

    class BoxPropertiesPrivate;

    using BOXPROPERTIES_HANDLER = void*;
    using CONST_BOXPROPERTIES_HANDLER = const void*;

    class IPGMARKINGLIBRARYWRAPPER_API BoxProperties {
        friend class PointList;
        friend class VectorList;

    private:
        BoxPropertiesPrivate* dPtr;

    private:
        CONST_BOXPROPERTIES_HANDLER getManagedPtr() const;

    public:
        BoxProperties(float size);
        BoxProperties(float width, float height);
        BoxProperties(float width, float height, float radius);
        BoxProperties(float width, float height, float radius, float segmentsPerCorner);
        ~BoxProperties();
        float getHeight() const;
        float getWidth() const;
        float getRadius() const;
        float getSegments() const;

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const BoxProperties& obj);

    };

}
