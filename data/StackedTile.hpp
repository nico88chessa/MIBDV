#ifndef STACKEDTILE_HPP
#define STACKEDTILE_HPP

#include <QList>
#include <QtMath>

#include <configure.h>

#include <Tile.hpp>
#include <ComputationUtils.hpp>

static const int NUM_POINT_PER_STACK_DEFAULT = 100;

namespace PROGRAM_NAMESPACE {

template <typename T>
class StackedTile {
private:
    QList<Tile<T>> list;
    int pointsPerTile;
    BoundingBoxI bb;

private:
    StackedTile(const PointI& min, const PointI& max, int pps) :
        list(), pointsPerTile(pps), bb(min, max) {
    }

    StackedTile(PointI p, int width, int height, int pps) :
        StackedTile(p, p + PointI(width, height), pps) { }

    StackedTile(const BoundingBoxI& b, int pps) :
        StackedTile(b.getMin(), b.getMax(), pps) { }

    StackedTile(int minX, int minY, int maxX, int maxY, int pps) :
        StackedTile(PointI(minX, minY), PointI(maxX, maxY), pps) { }

public:
    StackedTile() : list(), pointsPerTile(0), bb() { }

    StackedTile(const Tile<T>& tile, int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(tile.getBoundingBox(), pps) {

        const auto& ps = tile.getPointSet();

        if (ps.getMin() < bb.getMin() || ps.getMax() >= bb.getMax()) {
            traceErr() << "Impossibile inserire il PointSet in questo StackTile";
            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
            traceErr() << "PointSet: " << ps.getMin() << " x " << ps.getMax() ;
        }

        auto&& vector = ps.getVector();
        int currentIndex = 0;

        for (auto&& p: vector) {

            if (list.isEmpty()) {

                list.append(Tile<T>(bb, pointsPerTile));

            } else if (list.at(currentIndex).size() == pointsPerTile) {

                list.append(Tile<T>(bb, pointsPerTile));
                ++currentIndex;

            }

            list[currentIndex].addPoint(p);

        }

    }

    StackedTile(const Tile<T>& tile, int minDistanceXum, int minDistanceYum,
                int pps = NUM_POINT_PER_STACK_DEFAULT) :
        StackedTile(tile.getBoundingBox(), pps) {

        // TODO NIC 11/02/2018 - controllare che minDistanceX e minDistanceY siano valorizzti
        const auto& ps = tile.getPointSet();
        PointSet<T> psSorted = ComputationUtils::sortPointSet(ps);

        QVector<Point<T>> points = psSorted.getVector();
        int currentIndex = 0;

        BoundingBoxI currentBB;

        while (!points.isEmpty()) {

            bool canContinue = true;
            int i=0;

            while (canContinue) {

                auto point = points.at(i);

                // se la lista non esiste o e' vuota, allora inserisco il punto
                if (list.isEmpty() || (list.size() == currentIndex) || list.at(currentIndex).size() == 0) {

                    currentBB.setMin(PointI(point.getX(), point.getY()));
                    currentBB.setMax(point + PointI(minDistanceXum, minDistanceYum));

                    list.append(Tile<T>(bb, pointsPerTile));
                    list[currentIndex].addPoint(point);
                    points.removeAt(i);

                } else {

                    // controllo dove si trova il punto corrente rispetto alla buonding box attuale (bounding box per righe)
                    POINT_POSITION pos = currentBB.getPointPosition(point);

                    if (!(pos == POINT_POSITION::MIDDLE_RIGHT ||
                          pos == POINT_POSITION::TOP_RIGHT ||
                          pos == POINT_POSITION::TOP_MIDDLE)) {

                        // se non e' inseribile, passo al successivo
                        ++i;

                    } else {

                        // se e' inseribile, lo inserisco
                        list[currentIndex].addPoint(point);
                        points.removeAt(i);

                        // e verifico come aggiornare la bounding box;
                        // se mi sposto alla bounfing box lungo la x, devo aggiornare anche il minimo
                        // (altrimenti i punti verrebbero inseriti "a forma di L";
                        // se mi sposto alla bounding box lungo la y, allora aggiorno la
                        // bounding box normalmente

//                        if (point.getX() >= currentBB.getMax().getX()) {
                        if (pos == POINT_POSITION::TOP_RIGHT ||
                            pos == POINT_POSITION::MIDDLE_RIGHT ||
                            pos == POINT_POSITION::BOTTOM_RIGHT) {

                            currentBB.setMin(PointI(point.getX(), point.getY()));
                            currentBB.setMax(point + PointI(minDistanceXum, minDistanceYum));

                        } else {

                            int xPadding = point.getX() >= currentBB.getMax().getX() ? minDistanceXum : 0;
                            int yPadding = point.getY() >= currentBB.getMax().getY() ? minDistanceYum : 0;
                            currentBB.updateBoundingBox(point+PointI(xPadding, yPadding));

                        }

                    }

                }

                if (i==points.size())
                    canContinue = false;

            }
            ++currentIndex;

        }

        traceInfo() << list.size();

    }

    const QList<Tile<T>>& getTiles() const {
        return list;
    }

private:

//    bool addPoint(const Point<T>& p) {

//        if (p < bb.getMin() || p >= bb.getMax()) {

//            traceErr() << "Impossibile inserire il Point in questo StackTile";
//            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
//            traceErr() << "Point: " << p;
//            return false;

//        }

//        add(p);
//        return true;

//    }

//    bool addPointSet(const PointSet<T>& ps) {

//        if (ps.getMin() < bb.getMin() || ps.getMax() >= bb.getMax()) {
//            traceErr() << "Impossibile inserire il PointSet in questo StackTile";
//            traceErr() << "StackTile: " << bb.getMin() << " x " << bb.getMax() ;
//            traceErr() << "PointSet: " << ps.getMin() << " x " << ps.getMax() ;
//            return false;
//        }

//        auto&& vector = ps.getVector();

//        for (auto&& p: vector) {
//            if (list.isEmpty())
//                list.append(Tile<T>(bb, pointsPerTile));
//            else if (list.last().getPointSet().size() == pointsPerTile)
//                list.append(Tile<T>(bb, pointsPerTile));

//            list.last().addPoint(p);
//        }

//        return true;
//    }


//    inline void add(const Point<T>& p) {

//        if (list.isEmpty())
//            list.append(Tile<T>(bb, pointsPerTile));
//        else if (list.last().getPointSet().size() == pointsPerTile)
//            list.append(Tile<T>(bb, pointsPerTile));

//        list.last().addPoint(p);
//    }

};

using StackedTileI = StackedTile<int>;
using StackedTileUI = StackedTile<unsigned int>;
using StackedTileF = StackedTile<float>;
using StackedTileD = StackedTile<double>;

}

#endif // STACKEDTILE_HPP
