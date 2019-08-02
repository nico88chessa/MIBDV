#ifndef GRIDTILE_HPP
#define GRIDTILE_HPP

#include <Tile.hpp>


namespace PROGRAM_NAMESPACE {


/**
 * Una GridTile corrisponde ad una singola Tile di una griglia;
 * a differenza del Tile, la GridTile in piu' contiene le coordinate (row, column) del tile
 * all'interno della griglia (Grid)
 */
template <typename T>
class GridTile : public Tile<T> {
public:
    using Ptr = GridTile*;
    using ConstPtr = const GridTile*;

private:
    int rowIndex; // indice della riga all'interno della griglia
    int colIndex; // indice della colonna all'interno della griglia

public:

    GridTile() : Tile() {
        rowIndex = -1;
        colIndex = -1;
    }

    GridTile(const PointI& min, const PointI& max, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        Tile(min, max, estimatedPointSize) {

        rowIndex = -1;
        colIndex = -1;
    }

    GridTile(PointI p, int width, int height, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        GridTile(p, p + PointI(width, height), estimatedPointSize) { }

    GridTile(const BoundingBoxI& b, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        GridTile(b.getMin(), b.getMax(), estimatedPointSize) { }

    GridTile(int minX, int minY, int maxX, int maxY, int estimatedPointSize = DEFAULT_POINT_SET_SIZE) :
        GridTile(PointI(minX, minY), PointI(maxX, maxY), estimatedPointSize) { }

    int getRowIndex() const  { return rowIndex; }
    void setRowIndex(int value) { rowIndex = value; }

    int getColIndex() const { return colIndex; }
    void setColIndex(int value) { colIndex = value; }
};

using GridTileI = GridTile<int>;
using GridTileUI = GridTile<unsigned int>;
using GridTileF = GridTile<float>;
using GridTileD = GridTile<double>;


}

#endif // GRIDTILE_HPP
