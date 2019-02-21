#ifndef FILTERJSONPARSER_HPP
#define FILTERJSONPARSER_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QStringList>
#include <QString>
#include <QByteArray>

#include <Logger.hpp>

#include "AbstractJsonParser.hpp"
#include <data/Filter.hpp>

namespace PROGRAM_NAMESPACE {

static constexpr char* FILTER_JSON_NUMBER_OF_POINTS_KEY = "NumberOfPoints";
static constexpr char* FILTER_JSON_NUMBER_MAX_KEY = "Max";
static constexpr char* FILTER_JSON_NUMBER_MIN_KEY = "Min";
static constexpr char* FILTER_JSON_NUMBER_COORDINATE_KEY = "Coordinate";

class FilterJsonParser : public AbstractJsonParser<Filter> {
public:
    using Ptr = FilterJsonParser*;
    using ConstPtr = const FilterJsonParser*;

protected:
    virtual JsonParserError encodeJson(const Filter* obj, QByteArray& output) {

        // TODO NIC - 19/02/2018
        traceEnter;
        traceExit;
        return JSON_PARSER_NO_ERROR;

    }

    virtual JsonParserError decodeJson(const QByteArray& input, Filter* obj) {

        traceEnter;
        QJsonDocument doc = QJsonDocument::fromJson(input);
        if (doc.isNull()) {
            traceErr() << "Errore nella chiamata fromJson";
            return JSON_PARSER_ERROR_DOCUMENT_PARSER;
        }

        QJsonObject jsonObj = doc.object();

        QJsonValue numberOfPoints = jsonObj.value(FILTER_JSON_NUMBER_OF_POINTS_KEY);
        if (numberOfPoints.isUndefined()) {
            traceErr() << "Chiave NumberOfPoints non presente nel file json";
            return JSON_PARSER_ERROR_KEY_NOT_FOUND;
        }

        QJsonValue minPoint = jsonObj.value(FILTER_JSON_NUMBER_MIN_KEY);
        if (minPoint.isUndefined()) {
            traceErr() << "Chiave Min non presente nel file json";
            return JSON_PARSER_ERROR_KEY_NOT_FOUND;
        }

        QJsonValue maxPoint = jsonObj.value(FILTER_JSON_NUMBER_MAX_KEY);
        if (maxPoint.isUndefined()) {
            traceErr() << "Chiave Max non presente nel file json";
            return JSON_PARSER_ERROR_KEY_NOT_FOUND;
        }

        QJsonValue coordinate = jsonObj.value(FILTER_JSON_NUMBER_COORDINATE_KEY);
        if (coordinate.isUndefined()) {
            traceErr() << "Chiave Coordinate non presente nel file json";
            return JSON_PARSER_ERROR_KEY_NOT_FOUND;
        }

        // imposto il numero di punti
        obj->setNumOfPoints(numberOfPoints.toInt());

        // imposto il punto di minimo
        bool okX, okY;
        QStringList minSL = minPoint.toString().split(",");
        int x = minSL.at(0).toInt(&okX);
        int y = minSL.at(1).toInt(&okY);
        if (!okX || !okY) {
            traceErr() << "Errore nella conversione del punto min: " << minSL;
            return JSON_PARSER_ERROR_DATA_CONVERSION;
        }
        obj->setMin(x, y);

        // imposto il punto di masssimo
        QStringList maxSL = maxPoint.toString().split(",");
        x = maxSL.at(0).toInt(&okX);
        y = maxSL.at(1).toInt(&okY);
        if (!okX || !okY) {
            traceErr() << "Errore nella conversione del punto max: " << maxSL;
            return JSON_PARSER_ERROR_DATA_CONVERSION;
        }
        obj->setMax(x, y);

        // carico la lista di punti
        QJsonArray list = coordinate.toArray();
        for (const QJsonValue& value: list) {
            QStringList list = value.toString().split(",");
            bool okX, okY;
            x = list.at(0).toInt(&okX);
            y = list.at(1).toInt(&okY);
            if (!okX || !okY) {
                traceErr() << "Errore nella conversione del punto: " << list;
                return JSON_PARSER_ERROR_DATA_CONVERSION;
            }
            obj->addPoint(x, y);
        }

        traceExit;
        return JSON_PARSER_NO_ERROR;

    }

};

}

#endif // FILTERJSONPARSER_HPP
