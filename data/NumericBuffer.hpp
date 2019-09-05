#ifndef NUMERICEBUFFER_HPP
#define NUMERICEBUFFER_HPP

#include <type_traits>

#include <QVector>

#include <configure.h>


namespace PROGRAM_NAMESPACE {

template <typename T>
class NumericBuffer {
public:
    using Ptr = NumericBuffer*;
    using ConstPtr = const NumericBuffer*;

    static constexpr int DEFAULT_SAMPLE_ARRAY_SIZE = 10;

private:
    QVector<T> samples;
    T sum;
    int indexLastElement;
    int bufferSize;
    bool isBufferFull;

public:

    NumericBuffer(int bufferSize = DEFAULT_SAMPLE_ARRAY_SIZE) :
        bufferSize(bufferSize), samples(bufferSize, 0), sum(0), indexLastElement(0), isBufferFull(false) {

        static_assert(std::is_arithmetic_v<T>, "Il tipo di dato T deve essere numerico");

    }

    inline T getAverage() const {

        T average = 0;

        if (!isBufferFull)
            average = sum / (indexLastElement + 1);
        else
            average = sum / bufferSize;

        return average;

    }

    void addValue(T value) {

        if (isBufferFull) {

            T sampleToRemove = samples[indexLastElement];
            sum -= sampleToRemove;

        }

        sum += value;
        samples[indexLastElement] = value;

        if (!isBufferFull)
            isBufferFull = indexLastElement == (bufferSize - 1);

        indexLastElement = (indexLastElement + 1) % bufferSize;

    }

    int getSize() const { return bufferSize; }

};

}

#endif // NUMERICEBUFFER_HPP
