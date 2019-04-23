#pragma once

#include <iostream>
#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {

    using POINTWRAPPER_HANDLE_PTR = void*;
    using CONST_POINT_WRAPPER_HANDLE_PTR = const void*;

    class PointWrapperPrivate;
    //class PointList;
    //class PointListWrapper;
    //class Point;

    class IPGMARKINGLIBRARYWRAPPER_API PointWrapper {
        friend class PointList;
        friend class PointListWrapper;
        friend class Point;
        friend class Vector;
        friend class VectorWrapper;

    private:
        PointWrapperPrivate* dPtr;

    private:
        PointWrapper(POINTWRAPPER_HANDLE_PTR obj = nullptr);

    public:
        PointWrapper(const PointWrapper& other) = delete;
        PointWrapper(PointWrapper&& other);
        ~PointWrapper();

        PointWrapper& operator=(const PointWrapper& other) = delete;
        PointWrapper& operator=(PointWrapper&& other);

        Point clone();

        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getZ() const;
        void setZ(float z);

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const PointWrapper& obj);

    };

}
