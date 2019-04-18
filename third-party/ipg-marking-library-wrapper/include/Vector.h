#pragma once

#include "IpgmlDef.h"
#include "Point.h"
#include "PointWrapper.h"
#include <ostream>

namespace ipg_marking_library_wrapper {
    
    class VectorPrivate;

    using VECTOR_HANDLER = void*;
    using CONST_VECTOR_HANDLER = const void*;

    class IPGMARKINGLIBRARYWRAPPER_API Vector {
        friend class VectorList;

    private:
        VectorPrivate* dPtr;

    private:
        Vector(VECTOR_HANDLER obj);
        CONST_VECTOR_HANDLER getManagedPtr() const;

    public:
        Vector();
        Vector(const Vector& other);
        Vector(Vector&& other);
        //Vector(void* other); // copia l'oggetto
        Vector(const Point& start, const Point& end);
        ~Vector();

        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other); // clono l'oggetto

        float getLength() const;
        PointWrapper getStart() const;
        void setStart(const Point& start);
        PointWrapper getEnd() const;
        void setEnd(const Point& end);
        
        //void* getManagedPtr();
        //void releaseManagedPtr();

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const Vector& obj);

        static int getPointsPerVector();

    };

}
