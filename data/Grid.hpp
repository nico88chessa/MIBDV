#ifndef GRID_HPP
#define GRID_HPP

#include <QtMath>
#include <QVector>
#include <QList>

#include <configure.h>
#include <Tile.hpp>


namespace PROGRAM_NAMESPACE {

template <typename T>
using GridRow = QVector<Tile<T>>;

template <typename T>
class Grid {
public:
    using Ptr = Grid<T>*;
    using ConstPtr = const Grid<T>*;

private:
    QVector<GridRow<T>> vet;
    PointI start;

    int numTileX;
    int numTileY;
    int tileSize;

public:
    Grid(const PointI& _start, int width, int height, int _tileSize) :
        start(_start), vet(), tileSize(_tileSize) {

        traceEnter;

        numTileX = qCeil((float) width / tileSize);
        numTileY = qCeil((float) height / tileSize);

        /* NOTE NIC 07/02/2019 - punti bordo griglia
         * Questo serve per gestire i casi in cui i punti sono sul bordo della griglia
         */
        if (width % tileSize == 0) {
            traceDebug() << "Tile multiplo esatto lungo l'asse X; aggiungo un tile in X";
            numTileX++;
        }
        if (height % tileSize == 0) {
            traceDebug() << "Tile multiplo esatto lungo l'asse Y; aggiungo un tile in Y";
            numTileY++;
        }

        traceDebug() << "Grid width: " << width;
        traceDebug() << "Grid height" << height;
        traceDebug() << "Grid numero Tile X: " << numTileX;
        traceDebug() << "Grid numero Tile Y: " << numTileY;

        PointI m = start;
        for (int r=0; r<numTileX; ++r) {

            QVector<Tile<T>> row;
            m.setX(start.getX() + tileSize*r);

            for (int c=0; c<numTileY; ++c) {
                m.setY(start.getY() + tileSize*c);
                PointI M = m + PointI(tileSize, tileSize);
                Tile<T> t(m, M);
                traceDebug() << "Indice Tile X: " << r;
                traceDebug() << "Indice Tile Y: " << c;
                traceDebug() << "Tile: " << m << " x " << M;
                row.append(t);
            }

            vet.append(row);
        }

        traceExit;

    }

    Grid(int x, int y, int width, int height, int tileSize) :
        Grid(PointI(x, y), width, height, tileSize) { }

    const Tile<T>& getTile(int r, int c) const {
        return vet.at(r).at(c);
    }

    Tile<T>& getTile(int r, int c) {
        return vet[r][c];
    }

    const Tile<T>& getTile(const Point<T>& p) const {
        return getTile(qFloor((float) (p - start).getX()/tileSize), qFloor((float) (p - start).getY()/tileSize));
    }

    Tile<T>& getTile(const Point<T>& p) {
        return getTile(qFloor((float) (p - start).getX()/tileSize), qFloor((float) (p - start).getY()/tileSize));
    }

    const GridRow<T>& getRow(int row) const {
        return vet.at(row);
    }

    GridRow<T>& getRow(int row) {
        return vet[row];
    }

    void addPoint(const Point<T>& p) {
        auto&& tile = this->getTile(p);
        tile.addPoint(p);
    }

    int getRows() const { return numTileX; }

    int getCols() const { return numTileY; }

    inline int getTileIndexX (const Point<T>& p) const { return qFloor((float) (p - start).getX()/tileSize); }

    inline int getTileIndexY (const Point<T>& p) const { return qFloor((float) (p - start).getY()/tileSize); }

};

using GridI = Grid<int>;
using GridUI = Grid<unsigned int>;
using GridF = Grid<float>;
using GridD = Grid<double>;

using GridRowI = GridRow<int>;
using GridRowUI = GridRow<unsigned int>;
using GridRowF = GridRow<float>;
using GridRowD = GridRow<double>;

}

#endif // GRID_HPP
