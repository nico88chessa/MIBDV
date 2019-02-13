#ifndef POINT_HPP
#define POINT_HPP

#include <QDebug>
#include <configure.h>

namespace PROGRAM_NAMESPACE {

template <typename T>
class Point {
private:
    T x;
    T y;

public:
    Point() : x(0), y(0) { }
    Point(T x, T y) {
        this->x = x;
        this->y = y;
    }

    inline T getY() const { return y; }
    inline void setY(const T &value) { y = value; }
    inline T getX() const { return x; }
    inline void setX(const T &value) { x = value; }

    inline Point<T>& operator+=(const Point<T>& p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }
    inline Point<T>& operator-=(const Point<T>& p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }
    inline Point<T>& operator=(const Point<T>& p) {
        if (this != &p) {
            this->x = p.x;
            this->y = p.y;
        }
        return *this;
    }
    template <typename TT> friend inline bool operator<(const Point<TT>& l, const Point<TT>& r);
    template <typename TT> friend inline bool operator>(const Point<TT>& l, const Point<TT>& r);
    template <typename TT> friend inline bool operator<=(const Point<TT>& l, const Point<TT>& r);
    template <typename TT> friend inline bool operator>=(const Point<TT>& l, const Point<TT>& r);
    template <typename TT> friend inline bool operator==(const Point<TT>& l, const Point<TT>& r);
    template <typename TT> friend inline bool operator!=(const Point<TT>& l, const Point<TT>& r);
    template <typename TT> friend inline Point<TT> operator+(Point<TT> l, const Point<TT>& r);
    template <typename TT> friend inline Point<TT> operator-(Point<TT> l, const Point<TT>& r);
    template <typename TT> friend inline QDebug operator<<(QDebug dbg, const Point<TT>& p);

};

// TODO NIC 04/02/2019 - verificare il comportamento del confronto nel caso di numero float
template<typename TT>
bool operator<(const Point<TT>& l, const Point<TT>& r) {
    bool res;
    if (l.x < r.x)
        res = true;
    else if (l.x == r.x)
        res = l.y < r.y;
    else
        res = false;
    return res;
}

template<typename TT>
bool operator>(const Point<TT> &l, const Point<TT> &r) {
    bool res;
    if (l.x > r.x)
        res = true;
    else if (l.x == r.x)
        res = l.y > r.y;
    else
        res = false;
    return res;
}

template<typename TT>
bool operator<=(const Point<TT> &l, const Point<TT> &r) {
    return !(l > r);
}

template<typename TT>
bool operator>=(const Point<TT> &l, const Point<TT> &r) {
    return !(l < r);
}

template<typename TT>
bool operator==(const Point<TT> &l, const Point<TT> &r) {
    return (l.x == r.x) && (l.y == r.y);
}

template<typename TT>
bool operator!=(const Point<TT> &l, const Point<TT> &r) {
    return !(l==r);
}

template<typename TT>
QDebug operator<<(QDebug dbg, const Point<TT> &p) {
    dbg.nospace() << "(" << p.x << ", " << p.y << ")";
    return dbg;
}

template<typename TT>
Point<TT> operator+(Point<TT> l, const Point<TT>& r) {
    l += r;
    return l;
}

template<typename TT>
Point<TT> operator-(Point<TT> l, const Point<TT> &r) {
    l -= r;
    return l;
}

using PointI = Point<int>;
using PointUI = Point<unsigned int>;
using PointF = Point<float>;
using PointD = Point<double>;



}

#endif // POINT_HPP
