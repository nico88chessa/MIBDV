#ifndef GRIDROW_HPP
#define GRIDROW_HPP

#include <GridTile.hpp>


namespace PROGRAM_NAMESPACE {

template <typename T>
class GridRow {
public:
    using Ptr = GridRow*;
    using ConstPtr = const GridRow*;

    template <typename U> friend class Grid;
    friend class ComputationUtils;

private:
    QVector<GridTile<T>> tileList;

    // indica il numero massimo di tile in una riga (possibile);
    // non e' detto che questo corrisponda con il numero di tile presenti in row
    int maxCols;

public:
    GridRow() { }

    int getMaxCols() const { return maxCols; }
    void setMaxCols(int value) { maxCols = value; }

    const QVector<GridTile<T> >& getTileList() const { return tileList; }
    void setTileList(const QVector<GridTile<T> >& value) { tileList = value; }

    void addTile(const GridTile<T> tile) { tileList.append(tile); }

};

using GridRowI = GridRow<int>;
using GridRowUI = GridRow<unsigned int>;
using GridRowF = GridRow<float>;
using GridRowD = GridRow<double>;

}

#endif // GRIDROW_HPP
