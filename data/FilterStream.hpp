#ifndef FILTERSTREAM_HPP
#define FILTERSTREAM_HPP

#include "Filter.hpp"


namespace PROGRAM_NAMESPACE {

class FilterStream {
public:
    using Ptr = FilterStream*;
    using ConstPtr = const FilterStream*;

    class Stripe {
    private:
        int pointSize;
        BoundingBoxI boundingBox;
        QVector<PointI> points;

    public:
        Stripe(int defaultSize = 0);
        int getPointSize() const;
        void setPointSize(int value);
        const BoundingBoxI& getBoundingBox() const;
        BoundingBoxI& getBoundingBox();
        void setBoundingBox(const BoundingBoxI& value);
        void addPoint(const PointI& p);
        void addPoint(int x, int y);
        void reset();
        const QVector<PointI>& getPoints() const;

    };

private:
    int numOfPoints;
    BoundingBoxI boundingBox;
    int stripesNumber;
    int stripeWidthUm;
    QVector<Stripe> stripes;

public:
    FilterStream();
    int getNumOfPoints() const;
    void setNumOfPoints(int value);
    PointI getMax() const;
    void setMax(const PointI& value);
    void setMax(int x, int y);
    PointI getMin() const;
    void setMin(const PointI& value);
    void setMin(int x, int y);
    int getStripesNumber() const;
    void setStripesNumber(int value);
    int getStripeWidthUm() const;
    void setStripeWidthUm(int value);
    void setStripe(const Stripe& stripe, int index);
    const Stripe& getStripe(int index) const;

    friend inline QDebug operator<<(QDebug dbg, const FilterStream& fs);

};

}

#endif // FILTERSTREAM_HPP
