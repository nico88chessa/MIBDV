#pragma once

#include "IpgmlDef.h"

#include "Point.h"

#include <list>

namespace ipg_marking_library_wrapper {
    
    class PointListPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API PointList {
    private:
        PointListPrivate* dPtr;
    
    public:
        PointList();
        PointList(std::list<Point>& points);
        PointList(PointList&& other);
        ~PointList();
        int count() const;
        Point element(int i);
        void shift(float x, float y, float z);
        void rotate(double z);
        void rotate(double x, double y, double z);
        void rotate(float z);
        void rotate(float x, float y, float z);

        void* getManagedPtr();
        void releaseManagedPtr();

    };

}