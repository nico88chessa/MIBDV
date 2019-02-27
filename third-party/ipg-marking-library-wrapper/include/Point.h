#pragma once

#include "IpgmlDef.h"

namespace ipg_marking_library_wrapper {
    
    class PointPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API Point {
    private:
        PointPrivate* dPtr;

    public:
        Point();
        Point(const Point& other);
        Point(Point&& other);
        Point(void* obj);
        Point(float x, float y);
        Point(float x, float y, float z);
        ~Point();
        
        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getZ() const;
        void setZ(float z);

        void* getManagedPtr();
        void releaseManagedPtr();

        //void collect();
        
    };

}