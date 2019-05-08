#pragma once

#include "IpgmlDef.h"

#include "Point.h"
#include "PointWrapper.h"
#include "PolygonProperties.h"
#include "BoxProperties.h"

#include <list>

/*
 * Note Nic: la libreria IPG non permette di fare una copia della pointlist;
   il costruttore con parametro list<Point> usa ciascun punto come reference, non e' una deep copy;
   per fare una deep copy, e' indispensabile usare il metodo append
 */

namespace ipg_marking_library_wrapper {
    
    using POINTLIST_HANDLER = void*;
    using CONST_POINTLIST_HANDLER = const void*;
    
    class PointListPrivate;

    class IPGMARKINGLIBRARYWRAPPER_API PointList {
        friend class Scanner;

    private:
        PointListPrivate* dPtr;

    private:
        CONST_POINTLIST_HANDLER getManagedPtr() const;
    
    public:
        PointList();
        PointList(std::list<Point>& points);
        //PointList(const PointList& copy);
        PointList(PointList&& other);
        PointList(const PolygonProperties& polygonProperties);
        PointList(const BoxProperties& boxProperties);
        ~PointList();

        void append(const PointList& pl);
        int count() const;
        Point center() const;
        PointWrapper element(int i) const;
        void shift(float x, float y, float z);
        void rotate(double z);
        void rotate(double x, double y, double z);
        void rotate(float z);
        void rotate(float x, float y, float z);
        void reverseOrder();
        void scale(float scaleFactor);
        void scale(float x, float y, float z);
        std::list<PointWrapper> points();

        // metodi che accedono direttamente alla lista di punti;
        // nella libreria Ipg, la proprieta' e' public per cui posso
        // fare quello che voglio.
        // sarebbe da fare un wrapper della lista...  ma ci vuole tempo
        // e la cosa non e' immediata
        void addPoint(const Point& p);

        IPGMARKINGLIBRARYWRAPPER_API friend std::ostream& operator<<(std::ostream& os, const PointList& obj);

        /*** da verificare se puo' essere utile ***/
        // per funzionare, alla classe PointListWrapper bisogna mettere la classe PointList come friend
        // operator PointListWrapper () {
        //     return PointListWrapper(const_cast<POINTLIST_WRAPPER_HANDLER_PTR>(getManagedPtr()));
        // 
        // }

    };

}