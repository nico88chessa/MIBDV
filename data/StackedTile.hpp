#ifndef STACKTILE_HPP
#define STACKEDTILE_HPP

#include <QList>
#include <QtMath>

#include <configure.h>

#include <Tile.hpp>

namespace PROGRAM_NAMESPACE {

template <typename T>
class StackedTile {
private:
    QList<PointSet<T>> list;
    int pointsPerStack;
    int currentListIndex;
    BoundingBoxI bb;

    static const int NUM_POINT_PER_STACK_DEFAULT = 100;

public:

    StackedTile(const PointI& min, const PointI& max, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        list(), bb(min, max), pointsPerStack(pps) {
    }

    StackedTile(PointI p, int width, int height, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(p, p + PointI(width, height), pps) { }

    StackedTile(int minX, int minY, int maxX, int maxY, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(PointI(minX, minY), PointI(maxX, maxY), pps) { }

    StackedTile(const Tile<T>& tile,
              int pps = NUM_POINT_PER_STACK_DEFAULT) :
        pointsPerStack(pps),
        currentListIndex(0),
        bb(tile.getBoundingBox()) {

        auto&& pointSet = tile.getCPointSet();

        this->addPointSet(pointSet);

//        int listSize = qCeil((float) pointSet.size() / pointsPerStack);
//        for (int i=0; i<listSize; ++i)
//            list.append(PointSet<T>(pointsPerStack));

//        auto&& vector = pointSet.getVector();

//        int count = 0;
//        for (auto&& p: vector) {

//            if (count == pointsPerStack)
//                ++currentListIndex;

//            list[currentListIndex].addPoint(p);
//            ++count;

//        }

    }

    bool addPoint(const Point<T>& p) {

        if (p < bb.getMin() || p >= bb.getMax()) {

            traceErr() << "Impossibile inserire il Point in questo StackTile";
            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
            traceErr() << "Point: " << p;
            return false;

        }

        add(p);
//        if (list.at(currentListIndex).size() == pointsPerStack) {
//            list.append(PointSet<T>(pointsPerStack));
//            ++currentListIndex;
//        }

//        list[currentListIndex].addPoint(p);
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

    const QList<PointSet<T>>& getList() const { return list; }

    QList<PointSet<T>>& getList() { return list; }

protected:

    inline void add(const Point<T>& p) {

        if (list.at(currentListIndex).size() == pointsPerStack) {
            list.append(PointSet<T>(pointsPerStack));
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
