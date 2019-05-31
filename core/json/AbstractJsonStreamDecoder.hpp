#ifndef ABSTRACTJSONSTREAMDECODER_HPP
#define ABSTRACTJSONSTREAMDECODER_HPP

#include "configure.h"
#include "Constants.hpp"


namespace PROGRAM_NAMESPACE {

using JsonStreamDecoderError = int;

constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_NO_ERROR = 0;
constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_ERROR_DOCUMENT_PARSER = PROGRAM_ERR_START_CODE + 1; // 65537
constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_ERROR_KEY_NOT_FOUND = PROGRAM_ERR_START_CODE + 2;
constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_ERROR_DATA_CONVERSION = PROGRAM_ERR_START_CODE + 3;
constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_NAN_OBJECT = PROGRAM_ERR_START_CODE + 4;
constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_INVALID_CHUNK_ID = PROGRAM_ERR_START_CODE + 5;
constexpr static JsonStreamDecoderError JSON_STREAM_DECODER_INTERNAL_ERROR = PROGRAM_ERR_START_CODE + 6;

class IAbstractJsonStreamDecoder {
public:
    using Ptr = IAbstractJsonStreamDecoder*;
    using ConstPtr = const IAbstractJsonStreamDecoder*;

public:

    // NOTE NIC 02/05/2019 - non passo altri argomenti al di fuori dell'oggetto da valorizzare;
    // i dati potrebbero essere sia alcuni byte sia uno stream di dati;
    // non sapendo a priori cosa uso, lascio alle classi derivate questa scelta

    virtual JsonStreamDecoderError decodeHeader(void* object) = 0;

    virtual JsonStreamDecoderError decodeChunk(void* object, int index) = 0;

    virtual bool hasChunk(const void* object, int index) const = 0;

    virtual int chunkSize(const void* object) const = 0;

};

/**
 * NOTE NIC 07/05/2019 - sarebbe piu interessante usare la tecnica SFINAE per forzare il tipo T
 * ad avere i costrutti per gestire header e data; sarebbe interessante farlo in futuro
 */
template <typename T>
class AbstractJsonStreamDecoder : public IAbstractJsonStreamDecoder {
public:
    using Ptr = AbstractJsonStreamDecoder*;
    using ConstPtr = const AbstractJsonStreamDecoder*;

protected:

    virtual JsonStreamDecoderError decodeHeaderImpl(T* object) = 0;

    virtual JsonStreamDecoderError decodeChunkImpl(T* object, int index) = 0;

    virtual int chunkSizeImpl(const T* object) const = 0;

public:

    JsonStreamDecoderError decodeHeader(void* object) final {

        if (object == nullptr)
            return JSON_STREAM_DECODER_NAN_OBJECT;
        auto objCast = static_cast<T*>(object);

        return decodeHeaderImpl(objCast);

    }

    JsonStreamDecoderError decodeChunk(void* object, int index) final {

        if (object == nullptr)
            return JSON_STREAM_DECODER_NAN_OBJECT;

        auto objCast = static_cast<T*>(object);

        if (!hasChunk(objCast, index))
            return JSON_STREAM_DECODER_INVALID_CHUNK_ID;

        return decodeChunkImpl(objCast, index);

    }

    bool hasChunk(const void* object, int index) const final {

        if (object == nullptr)
            return false;
        auto objCast = static_cast<const T*>(object);
        return index < this->chunkSize(object);

    }

    int chunkSize(const void* object) const final {

        if (object == nullptr)
            return 0;

        auto objCast = static_cast<const T*>(object);
        return chunkSizeImpl(objCast);

    }

};

}

#endif // ABSTRACTJSONSTREAMDECODER_HPP
