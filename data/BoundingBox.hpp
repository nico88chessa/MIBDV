#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include <numeric>

#include <configure.h>

#include <Point.hpp>

namespace PROGRAM_NAMESPACE {

template <typename T>
class BoundingBox {
private:
    Point<T> m;
    Point<T> M;

public:
    BoundingBox() :
        m(std::numeric_limits<T>::max(), std::numeric_limits<T>::max()),
        M(std::numeric_limits<T>::min(), std::numeric_limits<T>::min()) {
    }

    BoundingBox(const Point<T>& _m, const Point<T>& _M) :
        m(_m),  M(_M) {
    }

    BoundingBox(T minX, T minY, T maxX, T maxY) :
        m(minX, minY),  M(maxX, maxY) {
    }

    inline const Point<T>& getMin() const { return m; }
    inline void setMin(const Point<T>& p) { m = p; }

    inline const Point<T>& getMax() const { return M; }
    inline void setMax(const Point<T>& p) { M = p; }

    inline void updateBoundingBox(const Point<T>& other) {

        if (other.getX() < m.getX()) m.setX(other.getX());
        if (other.getY() < m.getY()) m.setY(other.getY());

        if (other.getX() > M.getX()) M.setX(other.getX());
        if (other.getY() > M.getY()) M.setY(other.getY());

    }

};


using BoundingBoxI = BoundingBox<int>;
using BoundingBoxUI = BoundingBox<unsigned int>;
using BoundingBoxF = BoundingBox<float>;
using BoundingBoxD = BoundingBox<double>;

}

#endif // BOUNDINGBOX_HPP
