#pragma once

#include "IpgmlDef.h"
#include <iostream>


namespace ipg_marking_library_wrapper {

    class PolygonPropertiesPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API PolygonProperties {
    private:
        PolygonPropertiesPrivate* dPtr;

    public:
        PolygonProperties(int numberOfSides, float radius);
        ~PolygonProperties();
        float getNumberOfSides() const;
        float getRadius() const;

        void* getManagedPtr();
        void releaseManagedPtr();

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const PolygonProperties& obj);

    };

}
