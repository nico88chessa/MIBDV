#pragma once

#include "IpgmlDef.h"

#include "Point.h"
#include "PointWrapper.h"
#include "PolygonProperties.h"
#include "PointList.h"

#include <list>


namespace ipg_marking_library_wrapper {
    
    class PointListWrapperPrivate;

    using POINTLIST_WRAPPER_HANDLER_PTR = void*;

    class IPGMARKINGLIBRARYWRAPPER_API PointListWrapper {
    private:
        PointListWrapperPrivate* dPtr;
    
    private:
        PointListWrapper(POINTLIST_WRAPPER_HANDLER_PTR obj = nullptr);

    public:
        PointListWrapper(const PointWrapper& other) = delete;
        PointListWrapper(PointListWrapper&& other);
        ~PointListWrapper();

        PointListWrapper& operator=(const PointListWrapper& other) = delete;
        PointListWrapper& operator=(PointListWrapper&& other);

        void append(const PointListWrapper& pl);
        void append(const PointList& pl);
        int count() const;
        PointWrapper center() const;
        PointWrapper element(int i) const;
        void shift(float x, float y, float z);
        void rotate(double z);
        void rotate(double x, double y, double z);
        void rotate(float z);
        void rotate(float x, float y, float z);

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const PointListWrapper& obj);

    };

}