#include "Filter.hpp"

using namespace PROGRAM_NAMESPACE;

Filter::Filter() :
    numOfPoints(0), boundingBox(),
    points() {
}

int Filter::getNumOfPoints() const { return numOfPoints; }

void Filter::setNumOfPoints(int value) { numOfPoints = value; }

PointI Filter::getMax() const { return boundingBox.getMax(); }

void Filter::setMax(const PointI& value) { boundingBox.setMax(value); }

void Filter::setMax(int x, int y) { this->setMax(PointI(x, y)); }

PointI Filter::getMin() const { return boundingBox.getMin(); }

void Filter::setMin(const PointI& value) { boundingBox.setMin(value); }

void Filter::setMin(int x, int y) { this->setMin(PointI(x, y)); }

const QVector<PointI>& Filter::getPoints() const { return points; }

void Filter::setPoints(const QVector<PointI>& value) { points = value; }

void Filter::addPoint(const mibdv::PointI& p) {
    points.push_back(p);
}

void Filter::addPoint(int x, int y) {
    this->addPoint(PointI(x, y));
}

bool Filter::isValid() const {
    return numOfPoints == points.size();
}

QDebug operator<<(QDebug dbg, const Filter& f) {

    dbg.nospace() << "Numero di punti: " << f.getNumOfPoints();
    dbg.nospace() << "Min: " << f.getMin();
    dbg.nospace() << "Max: " << f.getMax();
    return dbg;

}
