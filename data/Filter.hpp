#ifndef FILTER_HPP
#define FILTER_HPP

#include <QVector>
#include <QDebug>

#include "configure.h"
#include <BoundingBox.hpp>


namespace PROGRAM_NAMESPACE {

class Filter {
public:
    using Ptr = Filter*;
    using ConstPtr = const Filter*;

private:
    int numOfPoints;
    BoundingBoxI boundingBox;
    QVector<PointI> points;

public:
    Filter();
    int getNumOfPoints() const;
    void setNumOfPoints(int value);
    PointI getMax() const;
    void setMax(const PointI& value);
    void setMax(int x, int y);
    PointI getMin() const;
    void setMin(const PointI& value);
    void setMin(int x, int y);
    const QVector<PointI>& getPoints() const;
    void setPoints(const QVector<PointI>& value);
    void addPoint(const PointI& p);
    void addPoint(int x, int y);
    bool isValid() const;

    friend inline QDebug operator<<(QDebug dbg, const Filter& p);

};

}

#endif // FILTER_HPP
