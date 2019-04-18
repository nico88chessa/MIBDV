#pragma once

#include "IpgmlDef.h"
#include <iostream>


namespace ipg_marking_library_wrapper {
    
    class PointPrivate;

    using POINT_HANDLER = void*;
    using CONST_POINT_HANDLER = const void*;

    class IPGMARKINGLIBRARYWRAPPER_API Point {
        friend class PointList;
        friend class VectorList;
        friend class PointWrapper;
        friend class Vector;

    private:
        PointPrivate* dPtr;

    private:
        Point(POINT_HANDLER obj);
        CONST_POINT_HANDLER getManagedPtr() const;

    public:
        Point();
        Point(const Point& other); // copio l'oggetto
        Point(Point&& other); // sposto l'oggetto
        //Point(void* other); // copio l'oggetto (uso Clone)
        Point(float x, float y);
        Point(float x, float y, float z);
        ~Point();

        Point& operator=(const Point& other); // clono l'oggetto
        Point& operator=(Point&& other);

        operator PointWrapper () const;
        
        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getZ() const;
        void setZ(float z);

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const Point& obj);
        
    };

}
