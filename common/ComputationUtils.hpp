#ifndef COMPUTATIONUTILS_HPP
#define COMPUTATIONUTILS_HPP

#include <QtAlgorithms>
#include <QList>
#include <QtMath>
#include <QVector>

#include <configure.h>

#include <algorithm>
#include <random>

#include <PointSet.hpp>
#include <Tile.hpp>
#include <StackedTile.hpp>
#include <Grid.hpp>


namespace PROGRAM_NAMESPACE {

class ComputationUtils {
public:

    template <typename T>
    static PointSet<T> sortPointSet(const PointSet<T>& other) {
        PointSet<T> temp = other;
        qSort(temp.set);
        return temp;
    }

    template <typename T>
    static PointSet<T> shufflePointSet(const PointSet<T>& other) {
        PointSet<T> temp = other;
        auto rng = std::default_random_engine{};
        std::shuffle(temp.set.begin(), temp.set.end(), rng);
        return temp;
    }

    template <typename T>
    static PointSet<T> movePointSet(const PointSet<T>& other, const Point<T> point) {
        PointSet<T> tempSet(other.set.size());
        foreach (const Point<T>& current, other.set)
            tempSet.addPoint(current + point);

        return tempSet;
    }

    template <typename T>
    static PointSet<T> moveXPointSet(const PointSet<T>& other, T offset) {
        return movePointSet(other, Point<T>(offset, 0));
    }

    template <typename T>
    static PointSet<T> moveYPointSet(const PointSet<T>& other, T offset) {
        return movePointSet(other, Point<T>(0, offset));
    }

    template <typename T>
    static PointSet<T> centerPointSet(const PointSet<T>& other, const Point<T>& center) {
        Point<T> move(-center.getX(), -center.getY());
        return movePointSet(other, move);
    }

    template <typename T>
    static QList<PointSet<T>> splitPointSet(const PointSet<T>& other, int pointPerSet) {

        int pointSetListSize = qCeil((float)other.size() / pointPerSet);
        QList<PointSet<T>> pointSetList;
        for (int i=0; i<pointSetListSize; ++i)
            pointSetList.append(PointSet<T>(pointPerSet));

        int count = 0;

        foreach (const Point<T>& p, other.set) {
            int pointSetIndex = count++ / pointPerSet;
            pointSetList[pointSetIndex].addPoint(p);
        }

        return pointSetList;
    }

    template <typename T>
    static QList<T> shuffleList(const QList<T>& other) {
        QList<T> temp = other;
        auto rng = std::default_random_engine{};
        std::shuffle(temp.begin(), temp.end(), rng);
        return temp;
    }

    template <typename T>
    static Tile<T> shuffleTile(const Tile<T>& other) {
        Tile<T> temp(other.getBoundingBox());
        temp.addPointSet(ComputationUtils::shufflePointSet(other.getPointSet()));
        return temp;
    }

    template <typename T>
    static GridRow<T> fromTopToBottom(const QVector<Tile<T>>& row) {
        GridRow<T> tilesInRow = row;
        std::sort(tilesInRow.begin(), tilesInRow.end(), [](const Tile<T>& l, const Tile<T>& r) -> bool {
            auto&& lMin = l.getBoundingBox().getMin();
            auto&& rMin = r.getBoundingBox().getMin();
            if (lMin.getX() < rMin.getX())
                return true;
            return lMin.getY() > rMin.getY();
        });
        return tilesInRow;
    }

};

}


#endif // COMPUTATIONUTILS_HPP
