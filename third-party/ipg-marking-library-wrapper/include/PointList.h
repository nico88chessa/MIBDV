#pragma once

#include "IpgmlDef.h"

#include "Point.h"
#include "PolygonProperties.h"

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
        PointList(const PolygonProperties& polygonProperties);
        ~PointList();
        void append(const PointList& pl);
        int count() const;
        Point center() const;
        Point element(int i) const;
        void shift(float x, float y, float z);
        void rotate(double z);
        void rotate(double x, double y, double z);
        void rotate(float z);
        void rotate(float x, float y, float z);

        void* getManagedPtr();
        void releaseManagedPtr();

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const PointList& obj);

    };

}