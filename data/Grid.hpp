#ifndef GRID_HPP
#define GRID_HPP

#include <QtMath>
#include <QVector>
#include <QList>

#include <configure.h>
#include <GridRow.hpp>


namespace PROGRAM_NAMESPACE {

//template <typename T>
//using GridRow = QVector<GridTile<T>>; // GridRow contiene tutti i tile di una singola riga della griglia

template <typename T>
class Grid {
public:
    using Ptr = Grid<T>*;
    using ConstPtr = const Grid<T>*;

private:
    QVector<GridRow<T>> vet; // contiene tutte le righe della griglia
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

        traceInfo() << "Grid width: " << width;
        traceInfo() << "Grid height" << height;
        traceInfo() << "Grid numero Tile X: " << numTileX;
        traceInfo() << "Grid numero Tile Y: " << numTileY;

        PointI m = start;
        for (int r=0; r<numTileX; ++r) {

            GridRow<T> row;
            m.setX(start.getX() + tileSize*r);

            row.setMaxCols(numTileY);

            for (int c=0; c<numTileY; ++c) {
                m.setY(start.getY() + tileSize*c);
                PointI M = m + PointI(tileSize, tileSize);
                GridTile<T> t(m, M);
                traceDebug() << "Indice Tile X: " << r;
                traceDebug() << "Indice Tile Y: " << c;
                traceDebug() << "Tile: " << m << " x " << M;
                t.setRowIndex(r);
                t.setColIndex(c);
                row.addTile(t);
            }

            vet.append(row);
        }

        traceExit;

    }

    Grid(int x, int y, int width, int height, int tileSize) :
        Grid(PointI(x, y), width, height, tileSize) { }

    PointI getStart() const { return start; }

    const GridTile<T>& getTile(int r, int c) const {
        return vet.at(r).getTileList().at(c);
    }

    GridTile<T>& getTile(int r, int c) {
        return vet[r].tileList[c];
    }

    const GridTile<T>& getTile(const Point<T>& p) const {
        return getTile(qFloor((float) (p - start).getX()/tileSize), qFloor((float) (p - start).getY()/tileSize));
    }

    GridTile<T>& getTile(const Point<T>& p) {
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

    inline int getTileSize() const { return tileSize; }

    inline int getTileIndexX (const Point<T>& p) const { return qFloor((float) (p - start).getX()/tileSize); }

    inline int getTileIndexY (const Point<T>& p) const { return qFloor((float) (p - start).getY()/tileSize); }

};

using GridI = Grid<int>;
using GridUI = Grid<unsigned int>;
using GridF = Grid<float>;
using GridD = Grid<double>;

}

#endif // GRID_HPP
