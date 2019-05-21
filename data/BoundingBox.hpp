#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include <numeric>

#include <configure.h>

#include <Point.hpp>


namespace PROGRAM_NAMESPACE {

enum class POINT_POSITION : int {
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    MIDDLE_LEFT,
    INSIDE,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

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

    bool isPointInside(const Point<T>& p) {
        return (p.getX()>=m.getX() && p.getY()>=m.getY()) &&
                (p.getX()<M.getX() && p.getY()<M.getY());
    }

    POINT_POSITION getPointPosition(const Point<T>& p) {

        T x = p.getX();
        T y = p.getY();

        T minX = m.getX();
        T minY = m.getY();

        T maxX = M.getX();
        T maxY = M.getY();

        if (x < minX) {

            if (y < minY)
                return POINT_POSITION::BOTTOM_LEFT;
            else if (y >= maxY)
                return POINT_POSITION::TOP_LEFT;
            else
                return POINT_POSITION::MIDDLE_LEFT;

        } else if (x >= maxX) {

            if (y < minY)
                return POINT_POSITION::BOTTOM_RIGHT;
            else if (y >= maxY)
                return POINT_POSITION::TOP_RIGHT;
            else
                return POINT_POSITION::MIDDLE_RIGHT;

        } else {

            if (y < minY)
                return POINT_POSITION::BOTTOM_MIDDLE;
            else if (y >= maxY)
                return POINT_POSITION::TOP_MIDDLE;
            else
                return POINT_POSITION::INSIDE;

        }

    }

    template <typename TT>
    friend inline bool operator==(const BoundingBox<TT>& l, const BoundingBox<TT>& r);

};


using BoundingBoxI = BoundingBox<int>;
using BoundingBoxUI = BoundingBox<unsigned int>;
using BoundingBoxF = BoundingBox<float>;
using BoundingBoxD = BoundingBox<double>;

template<typename TT>
bool operator==(const BoundingBox<TT>& l, const BoundingBox<TT>& r) {
    return (l.m == r.m) && (l.M == r.M);
}

}

#endif // BOUNDINGBOX_HPP
