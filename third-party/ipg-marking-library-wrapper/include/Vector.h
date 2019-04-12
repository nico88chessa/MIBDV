#pragma once

#include "IpgmlDef.h"
#include "Point.h"
#include <ostream>

namespace ipg_marking_library_wrapper {
    
    class VectorPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API Vector {
    private:
        VectorPrivate* dPtr;

    public:
        Vector();
        Vector(const Vector& other);
        Vector(Vector&& other);
        Vector(void* other); // copia l'oggetto
        Vector(const Point& start, const Point& end);
        ~Vector();

        Vector& operator=(const Vector& other);

        float getLength() const;
        Point getStart() const;
        void setStart(const Point& start);
        Point getEnd() const;
        void setEnd(const Point& end);
        
        void* getManagedPtr();
        void releaseManagedPtr();

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const Vector& obj);

        static int getPointsPerVector();

    };

}
