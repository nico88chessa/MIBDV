#ifndef STACKTILE_HPP
#define STACKEDTILE_HPP

#include <QList>
#include <QtMath>

#include <configure.h>

#include <Tile.hpp>

static const int NUM_POINT_PER_STACK_DEFAULT = 100;

namespace PROGRAM_NAMESPACE {

template <typename T>
class StackedTile {
private:
    QList<Tile<T>> list;
    int pointsPerTile;
    int currentListIndex;
    BoundingBoxI bb;

public:
    StackedTile() : list(), pointsPerTile(0), currentListIndex(0), bb() { }

    StackedTile(const PointI& min, const PointI& max, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        list(), pointsPerTile(pps), currentListIndex(0), bb(min, max) {
    }

    StackedTile(PointI p, int width, int height, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(p, p + PointI(width, height), pps) { }

    StackedTile(const BoundingBoxI& b, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(b.getMin(), b.getMax(), pps) { }

    StackedTile(int minX, int minY, int maxX, int maxY, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(PointI(minX, minY), PointI(maxX, maxY), pps) { }

    StackedTile(const Tile<T>& tile,
              int pps = NUM_POINT_PER_STACK_DEFAULT) :
        pointsPerTile(pps),
        currentListIndex(0),
        bb(tile.getBoundingBox()) {

        const auto& pointSet = tile.getPointSet();

        this->addPointSet(pointSet);

    }

    bool addPoint(const Point<T>& p) {

        if (p < bb.getMin() || p >= bb.getMax()) {

            traceErr() << "Impossibile inserire il Point in questo StackTile";
            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
            traceErr() << "Point: " << p;
            return false;

        }

        add(p);
        return true;

    }

    bool addPointSet(const PointSet<T>& ps) {

        if (ps.getMin() < bb.getMin() || ps.getMax() >= bb.getMax()) {
            traceErr() << "Impossibile inserire il PointSet in questo StackTile";
            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
            traceErr() << "PointSet: " << ps.getMin() << " x " << ps.getMax() ;
            return false;
        }

        auto&& vector = ps.getVector();

        for (auto&& p: vector)
            add(p);

        return true;
    }

    const QList<Tile<T>>& getTiles() const { return list; }

protected:

    inline void add(const Point<T>& p) {

        if (list.isEmpty()) {

            list.append(Tile<T>(bb, pointsPerTile));

        } else if (list.at(currentListIndex).getPointSet().size() == pointsPerTile) {

            list.append(Tile<T>(bb, pointsPerTile));
            ++currentListIndex;

        }

        list[currentListIndex].addPoint(p);
    }

};

using StackedTileI = StackedTile<int>;
using StackedTileUI = StackedTile<unsigned int>;
using StackedTileF = StackedTile<float>;
using StackedTileD = StackedTile<double>;

}

#endif // STACKEDTILE_HPP
