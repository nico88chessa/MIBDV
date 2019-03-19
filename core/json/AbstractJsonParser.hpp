#ifndef ABSTRACTJSONPARSER_HPP
#define ABSTRACTJSONPARSER_HPP

#include "configure.h"
#include "Constants.hpp"

#include <QByteArray>

namespace PROGRAM_NAMESPACE {

constexpr static int JSON_PARSER_NO_ERROR = 0;
constexpr static int JSON_PARSER_ERROR_DOCUMENT_PARSER = PROGRAM_ERR_START_CODE + 1;
constexpr static int JSON_PARSER_ERROR_KEY_NOT_FOUND = PROGRAM_ERR_START_CODE + 2;
constexpr static int JSON_PARSER_ERROR_DATA_CONVERSION = PROGRAM_ERR_START_CODE + 3;

using JsonParserError = int;

class IAbstractJsonParser {
public:
    using Ptr = IAbstractJsonParser*;
    using ConstPtr = const IAbstractJsonParser*;

public:

    virtual JsonParserError encodeJson(const void* object, QByteArray& res) = 0;

    virtual JsonParserError decodeJson(const QByteArray& res, void* object) = 0;

    virtual JsonParserError decodeJsonHeaderOnly(const QByteArray& res, void* object) = 0;

};


template <typename T>
class AbstractJsonParser : public IAbstractJsonParser {
public:
    using Ptr = AbstractJsonParser*;
    using ConstPtr = const AbstractJsonParser*;

protected:

    virtual JsonParserError encodeJson(const T* obj, QByteArray& output) = 0;

    virtual JsonParserError decodeJson(const QByteArray& input, T* obj) = 0;

    virtual JsonParserError decodeJsonHeaderOnly(const QByteArray& input, T* obj) = 0;

public:
    JsonParserError encodeJson(const void* object, QByteArray& output) final {
        auto objCast = static_cast<const T*>(object);
        return encodeJson(objCast, output);
    }

    JsonParserError decodeJson(const QByteArray& input, void* object) final {
        auto objCast = static_cast<T*>(object);
        return decodeJson(input, objCast);
    }

    JsonParserError decodeJsonHeaderOnly(const QByteArray& input, void* object) final {
        auto objCast = static_cast<T*>(object);
        return decodeJsonHeaderOnly(input, objCast);
    }

};

}

#endif // ABSTRACTJSONPARSER_HPP
