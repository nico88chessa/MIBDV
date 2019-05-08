#include "FilterStream.hpp"


using namespace PROGRAM_NAMESPACE;

FilterStream::FilterStream() :
    numOfPoints(0), boundingBox(),
    stripesNumber(0), stripeWidthUm(0) { }

int FilterStream::getNumOfPoints() const { return numOfPoints; }

void FilterStream::setNumOfPoints(int value) { numOfPoints = value; }

PointI FilterStream::getMax() const { return boundingBox.getMax(); }

void FilterStream::setMax(const PointI& value) { boundingBox.setMax(value); }

void FilterStream::setMax(int x, int y) { this->setMax(PointI(x, y)); }

PointI FilterStream::getMin() const { return boundingBox.getMin(); }

void FilterStream::setMin(const PointI& value) { boundingBox.setMin(value); }

void FilterStream::setMin(int x, int y) { this->setMin(PointI(x, y)); }

int FilterStream::getStripesNumber() const { return stripesNumber; }

void FilterStream::setStripesNumber(int value) { stripesNumber = value; }

int FilterStream::getStripeWidthUm() const { return stripeWidthUm; }

void FilterStream::setStripeWidthUm(int value) { stripeWidthUm = value; }

void FilterStream::addStripe(const FilterStream::Stripe& stripe) { this->stripes.append(stripe); }

const FilterStream::Stripe& FilterStream::getStripe(int index) const {
    return stripes.at(index);
}

FilterStream::Stripe::Stripe(int defaultSize) : points(), pointSize(0) {
    if (defaultSize > 0)
        points.reserve(defaultSize);
}

int FilterStream::Stripe::getPointSize() const { return pointSize; }

void FilterStream::Stripe::setPointSize(int value) {

    pointSize = value;
    if (points.size() < pointSize)
        points.reserve(pointSize);

}

const BoundingBoxI& FilterStream::Stripe::getBoundingBox() const { return boundingBox; }

BoundingBoxI& FilterStream::Stripe::getBoundingBox() { return boundingBox; }

void FilterStream::Stripe::setBoundingBox(const BoundingBoxI& value) { boundingBox = value; }

void FilterStream::Stripe::addPoint(const PointI& p) { points.push_back(p); }

void FilterStream::Stripe::addPoint(int x, int y) { this->addPoint(PointI(x, y)); }

void FilterStream::Stripe::reset() {

    pointSize = 0;
    boundingBox = BoundingBoxI();
    points.clear(); // da Qt5.6 non rilascia la memoria
    points.squeeze(); // questo rilascia la memoria

}

QDebug operator<<(QDebug dbg, const FilterStream& fs) {

    dbg.nospace() << "Numero di punti: " << fs.getNumOfPoints();
    dbg.nospace() << "Min: " << fs.getMin();
    dbg.nospace() << "Max: " << fs.getMax();
    dbg.nospace() << "Num Stripes: " << fs.getStripesNumber();
    dbg.nospace() << "StripeWidth [um]: " << fs.getStripeWidthUm();
    return dbg;

}
