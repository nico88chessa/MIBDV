#ifndef POINTSET_HPP
#define POINTSET_HPP

#include <QVector>
#include <QtAlgorithms>

#include <algorithm>
#include <random>

#include <Point.hpp>
#include <BoundingBox.hpp>

namespace PROGRAM_NAMESPACE {

static const int DEFAULT_POINT_SET_SIZE = 100;

class ComputationUtils;

template <typename T>
class PointSet {
    friend class ComputationUtils;

private:
    QVector<Point<T>> set;
    BoundingBox<T> boundingBox;

public:
    PointSet(int size = DEFAULT_POINT_SET_SIZE) :
        set() {

        set.reserve(size);
//        min.setX(std::numeric_limits<T>::max());
//        min.setY(std::numeric_limits<T>::max());
//        max.setX(std::numeric_limits<T>::min());
//        max.setY(std::numeric_limits<T>::min());
    }

    void addPoint(const Point<T>& p) {
        boundingBox.updateBoundingBox(p);
//        if (set.isEmpty()) {
//            min = p;
//            max = p;
//        } else {
//            if (p < min)
//                min = p;
//            if (p > max)
//                max = p;
//        }
        set.push_back(p);
    }

    void addPoint(T x, T y) {
        addPoint(Point<T>(x, y));
    }

    void removePoint(int position) {
        if (position < set.size()) {
            set.remove(position);
        }
    }

    void clear() {
        set.clear();
    }

    // TODO NIC 04/02/2019 - gestire con eccezione
    Point<T> getPoint(int position) const {
        if (position < set.size())
            return set.at(position);
        return Point<T>();
    }

//    void sortPointSet() {
//        qSort(set);
//    }

//    void shufflePointSet() {
//        auto rng = std::default_random_engine{};
//        std::shuffle(set.begin(), set.end(), rng);
//    }

    Point<T> getMin() const {
        if (set.size() > 0)
            return boundingBox.getMin();
        return Point<T>();
    }

    Point<T> getMax() const {
        if (set.size() > 0)
            return boundingBox.getMax();
        return Point<T>();
    }

    int size() const {
        return set.size();
    }

    const QVector<Point<T>>& getVector() const {
        return set;
    }

};

using PointSetI = PointSet<int>;
using PointSetUI = PointSet<unsigned int>;
using PointSetF = PointSet<float>;
using PointSetD = PointSet<double>;

}


#endif // POINTSET_HPP
