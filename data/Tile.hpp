#ifndef TILE_HPP
#define TILE_HPP

#include <QList>

#include <PointSet.hpp>
#include <Logger.hpp>

namespace PROGRAM_NAMESPACE {

class ComputationUtils;

template <typename T>
class Tile {
    friend class ComputationUtils;

private:
    PointSet<T> pointSet;
    BoundingBoxI bb;
    PointI center;

public:
    Tile() : pointSet(), bb(), center() { }

    Tile(const PointI& min, const PointI& max, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        pointSet(estimatedPointSize), bb(min, max) {

        center.setX((min.getX() + max.getX()) / 2);
        center.setY((min.getY() + max.getY()) / 2);
    }

    Tile(PointI p, int width, int height, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        Tile(p, p + PointI(width, height), estimatedPointSize) { }

    Tile(const BoundingBoxI& b, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        Tile(b.getMin(), b.getMax(), estimatedPointSize) { }

    Tile(int minX, int minY, int maxX, int maxY, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        Tile(PointI(minX, minY), PointI(maxX, maxY), estimatedPointSize) { }

    bool addPoint(const Point<T>& p) {

        if (p < bb.getMin() || p >= bb.getMax()) {
            traceErr() << "Impossibile inserire il Point in questo Tile";
            traceErr() << "Tile: " << bb.getMin() << " x " << bb.getMax() ;
            traceErr() << "Point: " << p;
            return false;
        } else {
            pointSet.addPoint(p);
            return true;
        }

    }

    bool addPointSet(const PointSet<T>& ps) {

        if (ps.getMin() < bb.getMin() || ps.getMax() >= bb.getMax()) {
            traceErr() << "Impossibile inserire il PointSet in questo Tile";
            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
            traceErr() << "PointSet: " << ps.getMin() << " x " << ps.getMax() ;
            return false;
        }

        auto&& vector = ps.getVector();

        for (auto&& p: vector)
            pointSet.addPoint(p);

        return true;
    }

    const BoundingBoxI& getBoundingBox() const {
        return bb;
    }

    void clearPointSet() {
        pointSet.clear();
    }

    const PointSet<T>& getPointSet() const {
        return pointSet;
    }

    const Point<T>& getCenter() const {
        return center;
    }

    int size() const {
        return this->pointSet.size();
    }

};

using TileI = Tile<int>;
using TileUI = Tile<unsigned int>;
using TileF = Tile<float>;
using TileD = Tile<double>;

}

#endif // TILE_HPP
