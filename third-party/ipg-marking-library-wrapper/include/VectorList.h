#pragma once

#include "IpgmlDef.h"

#include "Vector.h"
#include "PolygonProperties.h"

#include <list>


namespace ipg_marking_library_wrapper {

    using VECTORLIST_HANDLER = void*;
    using CONST_VECTORLIST_HANDLER = const void*;
    
    class VectorListPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API VectorList {
        friend class Scanner;

    private:
        VectorListPrivate* dPtr;
    
    private:
        CONST_VECTORLIST_HANDLER getManagedPtr() const;

    public:
        VectorList();
        VectorList(std::list<Vector>& points);
        //VectorList(const VectorList& other);
        VectorList(VectorList&& other);
        VectorList(const PolygonProperties& polygonProperties);
        ~VectorList();

        void append(const VectorList& vl);
        int count() const;
        Point center() const;
        Vector element(int i) const;
        void shift(float x, float y, float z);
        void rotate(double z);
        void rotate(double x, double y, double z);
        void rotate(float z);
        void rotate(float x, float y, float z);

        /*void* getManagedPtr();
        void releaseManagedPtr();*/

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const VectorList& obj);

    };

}