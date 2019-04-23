#pragma once

#include <iostream>
#include "IpgmlDef.h"
#include "PointWrapper.h"

namespace ipg_marking_library_wrapper {

    using VECTORWRAPPER_HANDLE_PTR = void*;
    using CONST_VECTORWRAPPER_HANDLE_PTR = const void*;

    class VectorWrapperPrivate;
    
    //class PointList;
    //class PointListWrapper;
    //class Point;

    class IPGMARKINGLIBRARYWRAPPER_API VectorWrapper {
        friend class VectorList;
        friend class VectorListWrapper;
        friend class Vector;
        friend class PointWrapper;
        friend class Point;

    private:
        VectorWrapperPrivate* dPtr;

    private:
        VectorWrapper(VECTORWRAPPER_HANDLE_PTR obj = nullptr);

    public:
        VectorWrapper(const VectorWrapper& other) = delete;
        VectorWrapper(VectorWrapper&& other);
        ~VectorWrapper();

        VectorWrapper& operator=(const VectorWrapper& other) = delete;
        VectorWrapper& operator=(VectorWrapper&& other);

        Vector clone();

        float getLength() const;
        PointWrapper getStart() const;
        void setStart(const Point& start);
        PointWrapper getEnd() const;
        void setEnd(const Point& end);

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const VectorWrapper& obj);

    };

}
