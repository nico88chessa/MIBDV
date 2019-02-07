#ifndef TILE_HPP
#define TILE_HPP

#include <QList>

#include <BoundingBox.hpp>
#include <PointSet.hpp>
#include <Logger.hpp>

namespace PROGRAM_NAMESPACE {

template <typename T>
class Tile {
private:
    PointSet<T> pointSet;
    BoundingBoxI bb;
    PointI center;

public:
    Tile() : pointSet(), bb(), center() { }

    Tile(const PointI& min, const PointI& max) :
        pointSet(), bb(min, max) {

        center.setX((min.getX() + max.getX()) / 2);
        center.setY((min.getY() + max.getY()) / 2);
    }

    Tile(PointI p, int width, int height) :
        Tile(p, p + PointI(width, height)) { }

    Tile(int minX, int minY, int maxX, int maxY) :
        Tile(PointI(minX, minY), PointI(maxX, maxY)) { }

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

    const BoundingBoxI& getBoundingBox() const {
        return bb;
    }

    void clearPointSet() {
        pointSet.clear();
    }

    PointSet<T>& getPointSet() {
        return pointSet;
    }

    const PointSet<T>& getCPointSet() const {
        return pointSet;
    }

};

using TileI = Tile<int>;
using TileUI = Tile<unsigned int>;
using TileF = Tile<float>;
using TileD = Tile<double>;

}

#endif // TILE_HPP
