#pragma once

#include "IpgmlDef.h"
#include <iostream>


namespace ipg_marking_library_wrapper {
    
    class PointPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API Point {
    private:
        PointPrivate* dPtr;

    public:
        Point();
        Point(const Point& other); // copio l'oggetto
        Point(Point&& other); // sposto l'oggetto
        Point(void* other); // copio l'oggetto (uso Clone)
        Point(float x, float y);
        Point(float x, float y, float z);
        ~Point();

        Point& operator=(const Point& other); // clono l'oggetto
        
        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);
        float getZ() const;
        void setZ(float z);

        void* getManagedPtr();
        void releaseManagedPtr();

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const Point& obj);
        
    };

}
