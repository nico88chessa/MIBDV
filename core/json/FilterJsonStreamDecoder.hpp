#ifndef FILTERJSONSTREAMDECODER_HPP
#define FILTERJSONSTREAMDECODER_HPP

#include <iostream>
#include <QFile>
#include <QStack>

#include "configure.h"
#include "AbstractJsonStreamDecoder.hpp"
#include <third-party/json-nlohmann/json.hpp>
#include <FilterStream.hpp>
#include <Logger.hpp>


namespace PROGRAM_NAMESPACE {

namespace nh = nlohmann;

class FilterHeaderDecoder : public nh::json_sax<nh::json> {
public:
    using Ptr = FilterHeaderDecoder*;
    using ConstPtr = const FilterHeaderDecoder*;

private:
    enum class OBJECT_TYPE {
        NO_OBJECTS,
        OBJECT_TO_SKIP,
        ROOT,
        NUMBER_OF_POINTS,
        BOUNDING_BOX,
        MIN,
        MAX,
        STRIPES,
        STRIPE_SIZE,
        STRIPE_WIDTH_UM,
        DATA_ARRAY,
        DATA_ARRAY_ELEMENT,
        POINTS,
        COORDINATE };

    static constexpr char* NO_OBJECTS_KEY = "Nok";
    static constexpr char* OBJECT_TO_SKIP_KEY = "ObjectToSkip";
    static constexpr char* ROOT_KEY = "Root";
    static constexpr char* NUMBER_OF_POINTS_KEY = "NumberOfPoints";
    static constexpr char* BOUNDING_BOX_KEY = "BoundingBox";
    static constexpr char* MIN_KEY = "Min";
    static constexpr char* MAX_KEY = "Max";
    static constexpr char* STRIPES_KEY = "Stripes";
    static constexpr char* STRIPE_WIDTH_UM_KEY ="StripeWidthUm";
    static constexpr char* STRIPE_SIZE_KEY = "Size";
    static constexpr char* DATA_ARRAY_KEY = "Data";
    static constexpr char* DATA_ARRAY_ELEMENT_KEY = "DataArrayElement";
    static constexpr char* POINTS_KEY = "Points";
    static constexpr char* COORDINATE_KEY = "Coordinate";
    static constexpr char* POINT_COORDINATES_SEPARATOR = ",";

private:
    FilterStream::Ptr f;
    QStack<OBJECT_TYPE> objectStack;
    OBJECT_TYPE currentKey;
    bool newObjectFound;
    int parseErrorCode;
    FilterStream::Stripe tempStripe;

    const bool onlyHeaderParse;
    const int chunkToParse;
    // indica il chunk a cui sto puntando (da vedere se e' da fare il parse o no)
    int parsingChunkId;

private:
    inline OBJECT_TYPE currentObject() const {
        return objectStack.isEmpty() ? OBJECT_TYPE::NO_OBJECTS : objectStack.last();
    }

    inline int currentIndex() const {
        return objectStack.size() - 1;
    }

    inline bool hasParent(int index) const {
        return !objectStack.isEmpty() && objectStack.at(index) != OBJECT_TYPE::ROOT;
    }

    inline OBJECT_TYPE parent(int index) const {
        if (!hasParent(index))
            return OBJECT_TYPE::NO_OBJECTS;
        return objectStack.at(index - 1);
    }

    inline int parentIndex(int index) const {
        if (!hasParent(index))
            return -1;
        return index - 1;
    }

    inline bool string2Point(QString str, PointI& dest) {

        QStringList pointSL = str.split(POINT_COORDINATES_SEPARATOR);
        bool okX, okY;
        int x = pointSL.at(0).toInt(&okX);
        int y = pointSL.at(1).toInt(&okY);
        if (!okX || !okY) {
            this->parseErrorCode = JSON_STREAM_DECODER_ERROR_DATA_CONVERSION;
            traceErr() << "Errore nella conversione del punto: " << pointSL;
            return false;
        }
        dest.setX(x);
        dest.setY(y);
        return true;
    }

public:

    FilterHeaderDecoder(FilterStream::Ptr filter) :
        f(filter), parseErrorCode(JSON_STREAM_DECODER_NO_ERROR),
        tempStripe(), onlyHeaderParse(true), chunkToParse(-1), parsingChunkId(-1) {
        newObjectFound = false;
    }

    FilterHeaderDecoder(FilterStream::Ptr filter, int chunkId) :
        f(filter), parseErrorCode(JSON_STREAM_DECODER_NO_ERROR),
        tempStripe(), onlyHeaderParse(false), chunkToParse(chunkId), parsingChunkId(-1) {

        newObjectFound = false;
    }

    ~FilterHeaderDecoder() { }

    bool null() {
        // NOTHING TO DO
        return true;
    }

    bool boolean(bool val) {
        // NOTHING TO DO
        return true;
    }

    bool number_integer(number_integer_t val) {
        // NOTHING TO DO
        return true;
    }

    bool number_unsigned(number_unsigned_t val) {

        if (currentObject() == OBJECT_TYPE::OBJECT_TO_SKIP)
            return true;

        switch (currentKey) {
        case OBJECT_TYPE::NUMBER_OF_POINTS: {

            if (currentObject() == OBJECT_TYPE::ROOT)
                f->setNumOfPoints(val);
            else if (currentObject() == OBJECT_TYPE::DATA_ARRAY_ELEMENT)
                this->tempStripe.setPointSize(val);
            break;

        }
        case OBJECT_TYPE::STRIPE_SIZE: {

            if (currentObject() == OBJECT_TYPE::STRIPES)
                f->setStripesNumber(val);
            break;

        }
        case OBJECT_TYPE::STRIPE_WIDTH_UM: {

            if (currentObject() == OBJECT_TYPE::STRIPES)
                f->setStripeWidthUm(val);
            break;

        }
        case OBJECT_TYPE::POINTS: {

            tempStripe.setPointSize(val);
            break;

        }
        default: break;
        }

        return true;

    }

    bool number_float(number_float_t val, const string_t& s) {
        return true;
    }

    bool string(string_t& val) {

        using namespace PROGRAM_NAMESPACE;

        if (currentObject() == OBJECT_TYPE::OBJECT_TO_SKIP)
            return true;

        switch (currentKey) {
        case OBJECT_TYPE::MIN: {

            QString valStr = QString::fromStdString(val);
            PointI p;
            if (!string2Point(valStr, p))
                return false;

            OBJECT_TYPE parent = this->parent(this->currentIndex());
            if (parent == OBJECT_TYPE::ROOT)
                f->setMin(p);
            else if (parent == OBJECT_TYPE::DATA_ARRAY_ELEMENT)
                this->tempStripe.getBoundingBox().setMin(p);

            break;

        }
        case OBJECT_TYPE::MAX: {

            QString valStr = QString::fromStdString(val);
            PointI p;
            if (!string2Point(valStr, p))
                return false;

            OBJECT_TYPE parent = this->parent(this->currentIndex());
            if (parent == OBJECT_TYPE::ROOT)
                f->setMax(p);
            else if (parent == OBJECT_TYPE::DATA_ARRAY_ELEMENT)
                this->tempStripe.getBoundingBox().setMax(p);

            break;

        }
        default: break;
        }

        switch (currentObject()) {

        case OBJECT_TYPE::COORDINATE: {

            QString valStr = QString::fromStdString(val);
            PointI p;
            if (!string2Point(valStr, p))
                return false;

            this->tempStripe.addPoint(p);

            break;

        }
        default: break;
        }

        return true;

    }

    bool start_object(size_t elements) {

        switch (currentObject()) {

        case OBJECT_TYPE::OBJECT_TO_SKIP: {
            objectStack.push(OBJECT_TYPE::OBJECT_TO_SKIP);
            break;
        }

        case OBJECT_TYPE::NO_OBJECTS: {
            objectStack.push(OBJECT_TYPE::ROOT);
            break;
        }

        case OBJECT_TYPE::DATA_ARRAY: {
            if (this->parsingChunkId == chunkToParse)
                this->currentKey = OBJECT_TYPE::DATA_ARRAY_ELEMENT;
            else
                this->currentKey = OBJECT_TYPE::OBJECT_TO_SKIP;
            ++parsingChunkId;
            tempStripe.reset();
            objectStack.push(this->currentKey);
            break;
        }

        default: {
            objectStack.push(this->currentKey);
            break;
        }

        }

        return true;
    }

    bool key(string_t& val) {

        if (currentObject() == OBJECT_TYPE::OBJECT_TO_SKIP)
            return true;

        // inizialmente imposto la chiave ad unknown
        this->currentKey = OBJECT_TYPE::NO_OBJECTS;
        auto valStr = QString::fromStdString(val);

        if (valStr.compare(OBJECT_TO_SKIP_KEY) == 0) this->currentKey = OBJECT_TYPE::OBJECT_TO_SKIP;
        else if (valStr.compare(ROOT_KEY) == 0) this->currentKey = OBJECT_TYPE::ROOT;
        else if (valStr.compare(NUMBER_OF_POINTS_KEY) == 0) this->currentKey = OBJECT_TYPE::NUMBER_OF_POINTS;
        else if (valStr.compare(BOUNDING_BOX_KEY) == 0) this->currentKey = OBJECT_TYPE::BOUNDING_BOX;
        else if (valStr.compare(MIN_KEY) == 0) this->currentKey = OBJECT_TYPE::MIN;
        else if (valStr.compare(MAX_KEY) == 0) this->currentKey = OBJECT_TYPE::MAX;
        else if (valStr.compare(STRIPES_KEY) == 0) this->currentKey = OBJECT_TYPE::STRIPES;
        else if (valStr.compare(STRIPE_SIZE_KEY) == 0) this->currentKey = OBJECT_TYPE::STRIPE_SIZE;
        else if (valStr.compare(DATA_ARRAY_KEY) == 0) this->currentKey = OBJECT_TYPE::DATA_ARRAY;
        else if (valStr.compare(DATA_ARRAY_ELEMENT_KEY) == 0) this->currentKey = OBJECT_TYPE::DATA_ARRAY_ELEMENT;
        else if (valStr.compare(STRIPE_WIDTH_UM_KEY) == 0) this->currentKey = OBJECT_TYPE::STRIPE_WIDTH_UM;
        else if (valStr.compare(POINTS_KEY) == 0) this->currentKey = OBJECT_TYPE::POINTS;
        else if (valStr.compare(COORDINATE_KEY) == 0) this->currentKey = OBJECT_TYPE::COORDINATE;

        if (this->currentKey == OBJECT_TYPE::NO_OBJECTS) {
            this->parseErrorCode = JSON_STREAM_DECODER_ERROR_KEY_NOT_FOUND;
            return false;
        }

        return true;

    }

    bool end_object() {

        switch (currentObject()) {

        case OBJECT_TYPE::DATA_ARRAY_ELEMENT: {
            f->addStripe(this->tempStripe);
            if (parsingChunkId > chunkToParse) {
                objectStack.pop();
                return false;
            }
            break;
        }

        case OBJECT_TYPE::OBJECT_TO_SKIP: {
            if (parent(currentIndex()) == OBJECT_TYPE::DATA_ARRAY)
                f->addStripe(FilterStream::Stripe());
            break;
        }

        default: break;
        }

        objectStack.pop();

        return true;
    }

    bool start_array(size_t elements) {

        if (currentObject() == OBJECT_TYPE::OBJECT_TO_SKIP)
            return true;

        if (this->currentKey == OBJECT_TYPE::DATA_ARRAY) {
            if (this->onlyHeaderParse)
                return false;
            parsingChunkId = 0;
        }

        objectStack.push(this->currentKey);
        return true;

    }

    bool end_array() {

        if (currentObject() == OBJECT_TYPE::OBJECT_TO_SKIP)
            return true;

        objectStack.pop();
        return true;
    }

    bool parse_error(size_t position, const std::string& last_token, const nlohmann::detail::exception& ex) {
        Q_UNUSED(position)
        Q_UNUSED(last_token)

        this->parseErrorCode = ex.id;

        return false;
    }

    int getParseErrorCode() const { return this->parseErrorCode; }

};


class FilterJsonStreamDecoder : public AbstractJsonStreamDecoder<FilterStream> {
public:
    using Ptr = FilterJsonStreamDecoder*;
    using ConstPtr = const FilterJsonStreamDecoder*;

private:
    const QString filepath;
    int chunkSize;

public:
    bool hasNextChunk() const {
        return true;
    }

    FilterJsonStreamDecoder(const QString& file) :
        filepath(file), chunkSize(0) { }

protected:

    JsonStreamDecoderError decodeHeaderImpl(FilterStream::Ptr object) override {

        if (!QFile::exists(filepath)) {
            traceErr() << "Il file " << filepath << "non esiste";
            return JSON_STREAM_DECODER_ERROR_DOCUMENT_PARSER;
        }

        std::ifstream inputStream(filepath.toStdString().c_str());

        FilterHeaderDecoder headerDecoder(object);
        bool res = nh::json::sax_parse(inputStream, &headerDecoder);
        if (!res) {
            int errorCode = headerDecoder.getParseErrorCode();
            if (errorCode != JSON_STREAM_DECODER_NO_ERROR) {
                if (errorCode < JSON_STREAM_DECODER_ERROR_DOCUMENT_PARSER) {
                    traceErr() << "Errore parser interno; codice errore: " << errorCode;
                    return JSON_STREAM_DECODER_INTERNAL_ERROR;
                } else {
                    traceErr() << "Errore nel parsing del file json; codice errore: " << errorCode;
                    return errorCode;
                }
            }
        }
        return JSON_STREAM_DECODER_NO_ERROR;

    }

    JsonStreamDecoderError decodeChunkImpl(FilterStream::Ptr object, int index) override {

        if (!QFile::exists(filepath)) {
            traceErr() << "Il file " << filepath << "non esiste";
            return JSON_STREAM_DECODER_ERROR_DOCUMENT_PARSER;
        }

        std::ifstream inputStream(filepath.toStdString().c_str());

        FilterHeaderDecoder headerDecoder(object, index);
        bool res = nh::json::sax_parse(inputStream, &headerDecoder);
        if (!res) {
            int errorCode = headerDecoder.getParseErrorCode();
            if (errorCode != JSON_STREAM_DECODER_NO_ERROR) {
                if (errorCode < JSON_STREAM_DECODER_ERROR_DOCUMENT_PARSER) {
                    traceErr() << "Errore parser interno; codice errore: " << errorCode;
                    return JSON_STREAM_DECODER_INTERNAL_ERROR;
                } else {
                    traceErr() << "Errore nel parsing del file json; codice errore: " << errorCode;
                    return errorCode;
                }
            }
        }
        return JSON_STREAM_DECODER_NO_ERROR;

    }

    int chunkSizeImpl(FilterStream::ConstPtr object) const override {

        return object->getStripesNumber();

    }

};


}

#endif // FILTERJSONSTREAMDECODER_HPP
