#ifndef GRID_HPP
#define GRID_HPP

#include <QtMath>
#include <QVector>
#include <QList>

#include <configure.h>

#include <Point.hpp>
#include <Tile.hpp>

namespace PROGRAM_NAMESPACE {

template <typename T>
class Grid {
private:
    QVector<QVector<Tile<T>>> vet;
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

    const Tile<T>& getCTile(int r, int c) const {
        return vet.at(r).at(c);
    }

    Tile<T>& getTile(int r, int c) {
        return vet[r][c];
    }

    const Tile<T>& getCTile(const Point<T>& p) const {
        return getCTile(qFloor((float) (p - start).getX()/tileSize), qFloor((float) (p - start).getY()/tileSize));
    }

    Tile<T>& getTile(const Point<T>& p) {
        return getTile(qFloor((float) (p - start).getX()/tileSize), qFloor((float) (p - start).getY()/tileSize));
    }

    const QVector<Tile<T>>& getCRow(int row) const {
        return vet.at(r);
    }

    QVector<Tile<T>>& getRow(int row) {
        return vet[r];
    }

    void addPoint(const Point<T>& p) {
        auto&& tile = this->getTile(p);
        tile.addPoint(p);
    }

};

using GridI = Grid<int>;
using GridUI = Grid<unsigned int>;
using GridF = Grid<float>;
using GridD = Grid<double>;

}

#endif // GRID_HPP
